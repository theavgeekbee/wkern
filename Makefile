CC = clang
LD = ld.lld
OBJCOPY = llvm-objcopy

ARCH_FLAG = \
	--target=riscv32-unknown-elf \
	-march=rv32im \
	-mabi=ilp32 \
	-D__SYS_LITTLE_ENDIAN__


COMMON_FLAGS = \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-exceptions \
	-fno-rtti \
	-fno-unwind-tables \
	-fno-asynchronous-unwind-tables \
	-nostdlib \
	-nostdinc \
	-nodefaultlibs \

KERNEL_FLAG = $(COMMON_FLAGS) -static
MODULE_FLAG = $(COMMON_FLAGS) -fPIC -fpie

SECTION_FLAG = \
	-ffunction-sections \
	-fdata-sections \
	-fno-common

INCLUDE_FLAG = -Iinclude -Ikernel
OPT_FLAG = -O2 -g3
WARN_FLAG = -Wall -Wextra -Werror -Wundef

KERNEL_LD_FLAG = -T linker.ld -nostdlib -m elf32lriscv --gc-sections -Map=$(MAP_FILE)
MODULE_LD_FLAG = -nostdlib -shared -m elf32lriscv --gc-sections

KERNEL_OBJCOPY_FLAG = -O binary \
		--strip-all \
		--remove-section=.comment \
		--remove-section=.debug* \
		--remove-section=.note* \
		--remove-section=.eh_frame \
		--remove-section=.eh_frame_ptr \
		--remove-section=.eh_frame_hdr

MODULE_OBJCOPY_FLAG = \
		--strip-debug \
		--remove-section=.comment \
		--remove-section=.note*

KERNEL_CFLAGS = $(ARCH_FLAG) $(KERNEL_FLAG) $(SECTION_FLAG) $(INCLUDE_FLAG) $(OPT_FLAG) $(WARN_FLAG)
MODULE_CFLAGS = $(ARCH_FLAG) $(MODULE_FLAG) $(SECTION_FLAG) $(INCLUDE_FLAG) $(OPT_FLAG) $(WARN_FLAG) -D__KERNEL_MODULE__

INCLUDE_DIR = include
KERNEL_DIR = kernel
OBJ_DIR = out/objects
BIN_DIR = out/bin
MODULE_BIN_DIR = out/modules

MODULES = $(filter-out kernel include out, $(shell find . -maxdepth 1 -type d -printf "%f\n" | grep -v "^\."))
MODULE_OBJ_DIR = $(addprefix $(OBJ_DIR)/, $(MODULES))
KERNEL_OBJ_DIR = $(OBJ_DIR)/kernel

KERNEL_C_SRCS = $(wildcard kernel/*.c)
KERNEL_ASM_SRCS = $(wildcard kernel/*.S)
KERNEL_C_OBJS = $(addprefix $(OBJ_DIR)/kernel/, $(notdir $(KERNEL_C_SRCS:.c=.o)))
KERNEL_ASM_OBJS = $(addprefix $(OBJ_DIR)/kernel/, $(notdir $(KERNEL_ASM_SRCS:.S=.o)))
KERNEL_OBJS = $(KERNEL_C_OBJS) $(KERNEL_ASM_OBJS)

define module_template
MODULE_$(1)_C_SRCS = $$(wildcard $(1)/*.c)
MODULE_$(1)_ASM_SRCS = $$(wildcard $(1)/*.S)
MODULE_$(1)_C_OBJS = $$(addprefix $(OBJ_DIR)/$(1)/, $$(notdir $$(MODULE_$(1)_C_SRCS:.c=.o)))
MODULE_$(1)_ASM_OBJS = $$(addprefix $(OBJ_DIR)/$(1)/, $$(notdir $$(MODULE_$(1)_ASM_SRCS:.S=.o)))
MODULE_$(1)_OBJS = $$(MODULE_$(1)_C_OBJS) $$(MODULE_$(1)_ASM_OBJS)
MODULE_$(1)_KO = $(MODULE_BIN_DIR)/$(1).ko
endef

$(foreach module,$(MODULES),$(eval $(call module_template,$(module))))

ALL_MODULES = $(foreach module,$(MODULES),$(MODULE_BIN_DIR)/$(module).ko)

KERNEL_ELF = $(BIN_DIR)/wkern.elf
KERNEL_BIN = $(BIN_DIR)/wkern.bin
MAP_FILE = $(BIN_DIR)/kernel.map
ALL_MOODULES = $(foreach module,$(MODULES),$(MODULE_BIN_DIR)/$(module).ko)

MODULE_OBJ_DIRS = $(addprefix $(OBJ_DIR)/, $(MODULES))
ALL_DIRS = $(OBJ_DIR)/kernel $(MODULE_OBJ_DIRS) $(BIN_DIR) $(MODULE_BIN_DIR)

.PHONY: all clean kernel modules $(MODULES)
.SILENT:
all: kernel modules
kernel: $(KERNEL_BIN)
modules: $(ALL_MODULES)
$(MODULES): %: $(MODULE_BIN_DIR)/%.ko

$(OBJ_DIR)/kernel/%.o: $(KERNEL_DIR)/%.c | $(OBJ_DIR)/kernel
	@echo " CC $<"
	$(CC) $(KERNEL_CFLAGS) -c $< -o $@

$(OBJ_DIR)/kernel/%.o: $(KERNEL_DIR)/%.S | $(OBJ_DIR)/kernel
	@echo " AS $<"
	$(CC) $(KERNEL_CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJS) linker.ld | $(BIN_DIR)
	@echo " LD $@"
	$(LD) $(KERNEL_LD_FLAG) $(KERNEL_OBJS) -o $@

$(KERNEL_BIN): $(KERNEL_ELF) | $(BIN_DIR)
	@echo " OBJCOPY $@"
	$(OBJCOPY) $(KERNEL_OBJCOPY_FLAG) $(KERNEL_ELF) $(KERNEL_BIN)

$(OBJ_DIR)/%.o: %.c# | $(OBJ_DIR)/%
	@mkdir -p $(dir $@)
	@echo " CC $@"
	$(CC) $(MODULE_CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.S# | $(OBJ_DIR)/%
	@mkdir -p $(dir $@)
	@echo " AS $@"
	$(CC) $(MODULE_CFLAGS) -c $< -o $@

define module_link_rule
$(MODULE_BIN_DIR)/$(1).ko: $$(MODULE_$(1)_OBJS) | $(MODULE_BIN_DIR)
	@echo " LD $$@"
	$$(LD) $$(MODULE_LD_FLAG) $$^ -o $$@
	@echo " OBJCOPY $$@"
	$$(OBJCOPY) $$(MODULE_OBJCOPY_FLAG) $$@ $$@
endef

$(foreach module,$(MODULES),$(eval $(call module_link_rule,$(module))))

$(ALL_DIRS):
	mkdir -p $@

clean:
	@echo " CLEAN out/"
	rm -rf out
