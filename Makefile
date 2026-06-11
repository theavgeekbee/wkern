CC = clang
LD = ld.lld
OBJCOPY = llvm-objcopy

ARCH_FLAG = \
   --target=riscv32-unknown-elf \
   -march=rv32im \
	-mabi=ilp32

KERNEL_FLAG = \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-exceptions \
	-fno-rtti \
	-fno-unwind-tables \
	-fno-asynchronous-unwind-tables \
	-nostdlib \
	-nostdinc \
	-static \
	-nodefaultlibs

SECTION_FLAG = \
	-ffunction-sections \
	-fdata-sections \
	-fno-common

INCLUDE_FLAG = -Iinclude -Isrc
OPT_FLAG = -O2 -g3
WARN_FLAG = -Wall -Wextra -Werror -Wundef

CFLAGS = $(ARCH_FLAG) $(KERNEL_FLAG) $(SECTION_FLAG) $(INCLUDE_FLAG) $(OPT_FLAG) $(WARN_FLAG)

C_SRCS = $(wildcard src/*.c)
ASM_SRCS = $(wildcard src/*.S)

C_OBJS = $(notdir $(C_SRCS:.c=.o))
ASM_OBJS = $(notdir $(ASM_SRCS:.S=.o))
OBJS = $(C_OBJS) $(ASM_OBJS)

KERNEL_ELF = out/bin/wkern.elf
KERNEL_BIN = out/bin/wkern.bin
MAP_FILE = out/bin/kernel.map

OBJ_DIR = out/objects
BIN_DIR = out/bin
OBJS_PATH = $(addprefix $(OBJ_DIR)/, $(OBJS))

LD_FLAG = -T linker.ld -fuse-ld=lld -Wl,-nostdlib -Wl,-melf32lriscv -Wl,--gc-sections -Wl,-Map=$(MAP_FILE)
OBJCOPY_FLAG = -O binary \
		--strip-all \
		--remove-section=.comment \
		--remove-section=.debug* \
		--remove-section=.note* \
		--remove-section=.eh_frame \
		--remove-section=.eh_frame_ptr \
		--remove-section=.eh_frame_hdr \


.PHONY: all clean
all: $(KERNEL_BIN)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@echo " CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.S | $(OBJ_DIR)
	@echo " AS $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(OBJS_PATH) linker.ld | $(BIN_DIR)
	@echo " LD $@"
	$(CC) $(CFLAGS) $(LD_FLAG) $(OBJS_PATH) -o $@

$(KERNEL_BIN): $(KERNEL_ELF) | $(BIN_DIR)
	@echo " OBJCOPY $@"
	$(OBJCOPY) $(OBJCOPY_FLAG) $(KERNEL_ELF) $(KERNEL_BIN)

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf out 

