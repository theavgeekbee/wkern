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

KERNEL_LD_FLAG = -T $(KERNEL_LD) -nostdlib -m elf32lriscv --gc-sections
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

MODULES = $(wildcard $(shell grep '^modules:' BuildConfig | sed 's/^modules://' | awk '{$$1=$$1; print}'))
MODULE_OBJECTS = $(foreach m,$(MODULES),$m/$m.ko)

define module_template
$(1)_SRCS = $$(wildcard $$(shell grep '^$(1):' BuildConfig | sed 's/^$(1)://' | awk '{$$$$1=$$$$1; print}'))
$(1)_ELF = $(1)/$(1).elf
$(1)_BIN = $(1)/$(1).ko

$(1)_C_SRCS = $$(filter %.c,$$($(1)_SRCS))
$(1)_S_SRCS = $$(filter %.S,$$($(1)_SRCS))
$(1)_C_OBJS = $$($(1)_C_SRCS:.c=.o)
$(1)_S_OBJS = $$($(1)_S_SRCS:.S=.o)
$(1)_OBJS = $$($(1)_C_OBJS) $$($(1)_S_OBJS)

$$($(1)_C_OBJS): %.o: %.c
	@echo " CC [MOD] $$<"
	$$(CC) $$(MODULE_CFLAGS) -c $$< -o $$@

$$($(1)_S_OBJS): %.o: %.S
	@echo " AS [MOD] $$<"
	$$(CC) $$(MODULE_CFLAGS) -c $$< -o $$@

$$($(1)_ELF): $$($(1)_OBJS)
	@echo " LD [MOD] $$@"
	$$(LD) $$(MODULE_LD_FLAG) $$^ -o $$@
$$($(1)_BIN): $$($(1)_ELF)
	@echo " OBJCOPY [MOD] $$@"
	$$(OBJCOPY) $$(MODULE_OBJCOPY_FLAG) $$^ $$@
endef
$(foreach module,$(MODULES),$(eval $(call module_template,$(module))))

KERNEL_SRCS = $(wildcard $(shell grep '^kernel:' BuildConfig | sed 's/^kernel://' | awk '{$$1=$$1; print}'))
KERNEL_LD := $(shell grep '^linker_script:' BuildConfig | sed 's/^linker_script://' | awk '{$$1=$$1; print}')
KERNEL_OBJS = $(KERNEL_SRCS:.c=.o)
KERNEL_OBJS := $(KERNEL_OBJS:.S=.o)
KERNEL_BIN = kernel/kernel.bin
KERNEL_ELF = kernel/kernel.elf

GENERATED = \
	$(KERNEL_OBJS) \
	$(KERNEL_ELF) \
	$(KERNEL_BIN) \
	$(foreach m,$(MODULES),$(m)/$(m).elf) \
	$(foreach m,$(MODULES),$(m)/$(m).ko) \
	$(foreach m,$(MODULES),$($(m)_OBJS))

.PHONY: all kernel clean modules $(MODULES)
.SILENT:
all: kernel modules
kernel: $(KERNEL_BIN)
modules: $(MODULES)
$(MODULES): $(MODULE_OBJECTS)

%.o: %.c
	@echo " CC $<"
	$(CC) $(KERNEL_CFLAGS) -c $< -o $@
%.o: %.S
	@echo " AS $<"
	$(CC) $(KERNEL_CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJS) $(KERNEL_LD) | $(KERNEL_OUTDIR)/
	@echo " LD $@"
	$(LD) $(KERNEL_LD_FLAG) $(KERNEL_OBJS) -o $@

$(KERNEL_BIN): $(KERNEL_ELF) | $(KERNEL_OUTDIR)/
	@echo " OBJCOPY $@"
	$(OBJCOPY) $(KERNEL_OBJCOPY_FLAG) $(KERNEL_ELF) $(KERNEL_BIN)

clean:
	@echo " CLEAN $(GENERATED)"
	$(RM) $(GENERATED)
