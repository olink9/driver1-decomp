# Driver (PS1, SLUS-008.42) - build the executable from split asm (and, as functions get
# matched, from C). Run inside distrobox `build`.
BASENAME := slus_008.42
BUILD    := build
CROSS    := mips-linux-gnu-
AS       := $(CROSS)as
LD       := $(CROSS)ld
OBJCOPY  := $(CROSS)objcopy
PYTHON   := .venv/bin/python

ASFLAGS  := -EL -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -G0
CC1      := compilers/gcc-2.7.2-cdk/cc1
CPP      := compilers/gcc-2.7.2-psx/cpp
CFLAGS   := -O2 -G8 -mips1 -mcpu=3000 -fgnu-linker -mgas -msoft-float -quiet -funsigned-char -w
CPPFLAGS := -Iinclude -Isrc -undef -Wall -lang-c -fno-builtin -Dmips -D__GNUC__=2 -D__OPTIMIZE__ -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D_LANGUAGE_C -DLANGUAGE_C -DNO_LOGS -DHACKS -DUSE_INCLUDE_ASM -D_internal_version_$(VERSION) -DSOTN_STR
MASPSX   := MASPSX_EXTERN_G=4 $(PYTHON) tools/maspsx/maspsx.py --aspsx-version=2.79 --expand-div -G8

S_FILES  := $(shell find asm -name '*.s' -not -path 'asm/nonmatchings/*')
C_FILES  := $(shell find src -name '*.c' -not -path 'src/pilot/*' 2>/dev/null)   # pilot files are standalone, not linked yet
O_FILES  := $(S_FILES:%.s=$(BUILD)/%.o) $(C_FILES:%.c=$(BUILD)/%.o)

all: $(BUILD)/SLUS_008.42
	@sha1sum -c $(BASENAME).sha1

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CFLAGS) | $(MASPSX) | $(AS) $(ASFLAGS) -o $@

$(BUILD)/$(BASENAME).elf: $(O_FILES) $(BASENAME).ld
	$(LD) -EL -T undefined_syms_auto.txt -T undefined_syms_manual.txt -T undefined_funcs_auto.txt -T $(BASENAME).ld -Map $(BUILD)/$(BASENAME).map --no-check-sections -o $@

$(BUILD)/SLUS_008.42: $(BUILD)/$(BASENAME).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD)
.PHONY: all clean
