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

OVERLAYS := frontend mcard cred iview
S_FILES  := $(shell find asm -name '*.s' -not -path 'asm/nonmatchings/*' $(foreach o,$(OVERLAYS) nfmv,-not -path 'asm/$(o)/*'))
O_FILES  := $(S_FILES:%.s=$(BUILD)/%.o)
# Real translation units (src/<file>.c, made with tools/mktu.py; src/pilot/ is per-function
# and not linked here). While there are none, main links straight from the split asm.
# Each one replaces the asm of the functions it defines: tools/linkcheck.py --link compiles it
# (the same cpp/cc1/maspsx/as pipeline as every pilot check), maps its .text onto that block
# range, places its .rodata/.sdata/.sbss at the original addresses and links the rest from
# the same asm. The sha1 check below stays the arbiter.
# only committed TUs: agents keep work-in-progress files in src/ while they merge
# original assembler modules (the HANDWRITTEN ASM runs) are TUs too: src/*.s, assembled as they are
TU_FILES := $(shell git ls-files ':(glob)src/*.c' ':(glob)src/*.s' 2>/dev/null)

all: $(BUILD)/SLUS_008.42
	@sha1sum -c $(BASENAME).sha1

# Code overlays (frontend.yaml / mcard.yaml / cred.yaml / iview.yaml): each its own binary at its
# load address. CRED and IVIEW share 0x1E0000 (never resident together; separate symbol files).
# References into main resolve through build/main_syms.ld (PROVIDE list exported from
# main's ELF by tools/mainsyms.py); FRONTEND <-> MCARD calls through splat's
# undefined_*_auto.<overlay>.txt.
# Overlay TUs: committed src/<overlay>/*.c (and src/nfmv/*.c) replace their asm the same way
# main's src/*.c do (linkcheck --target <overlay> --link, workdir build/tulink-<overlay>);
# with none committed the overlay links straight from the split asm as before.
overlays: $(foreach o,$(OVERLAYS),$(o)) nfmv
frontend: $(BUILD)/FRONTEND.BIN
	@sha1sum -c frontend.sha1
mcard: $(BUILD)/MCARD.BIN
	@sha1sum -c mcard.sha1
cred: $(BUILD)/CRED.BIN
	@sha1sum -c cred.sha1
iview: $(BUILD)/IVIEW.BIN
	@sha1sum -c iview.sha1

$(BUILD)/main_syms.ld: $(BUILD)/$(BASENAME).elf tools/mainsyms.py
	$(PYTHON) tools/mainsyms.py

define OVERLAY_RULES
$(1)_S := $$(shell find asm/$(1) -name '*.s' 2>/dev/null)
$(1)_O := $$($(1)_S:%.s=$(BUILD)/$(1)/%.o)
$(BUILD)/$(1)/%.o: %.s
	@mkdir -p $$(dir $$@)
	$$(AS) $$(ASFLAGS) -o $$@ $$<
$(1)_TU := $$(shell git ls-files ':(glob)src/$(1)/*.c' 2>/dev/null)
$(BUILD)/$(1).elf: $$($(1)_O) $(1).ld $(BUILD)/main_syms.ld $$($(1)_TU) $$(if $$($(1)_TU),$$(shell find include -name '*.h') tools/linkcheck.py tools/fnscore.py)
	$$(if $$($(1)_TU),$$(PYTHON) tools/linkcheck.py --target $(1) -j 2 --workdir $(BUILD)/tulink-$(1) --link $$@ --tu $$($(1)_TU),$$(LD) -EL -T $(BUILD)/main_syms.ld -T undefined_syms_auto.$(1).txt -T undefined_funcs_auto.$(1).txt -T $(1).ld -Map $(BUILD)/$(1).map --no-check-sections -o $$@)
$(BUILD)/$(2): $(BUILD)/$(1).elf
	$$(OBJCOPY) -O binary $$< $$@
endef
$(eval $(call OVERLAY_RULES,frontend,FRONTEND.BIN))
$(eval $(call OVERLAY_RULES,mcard,MCARD.BIN))
$(eval $(call OVERLAY_RULES,cred,CRED.BIN))
$(eval $(call OVERLAY_RULES,iview,IVIEW.BIN))

# NFMV.EXE (nfmv.yaml): the FMV player, a separate PS-X EXE at 0x80100000 with its own
# PsyQ libraries and gp - it does not link against main (no main_syms.ld).
nfmv: $(BUILD)/NFMV.EXE
	@sha1sum -c nfmv.sha1
nfmv_S := $(shell find asm/nfmv -name '*.s' 2>/dev/null)
nfmv_O := $(nfmv_S:%.s=$(BUILD)/nfmv/%.o)
$(BUILD)/nfmv/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<
nfmv_TU := $(shell git ls-files ':(glob)src/nfmv/*.c' 2>/dev/null)
$(BUILD)/nfmv.elf: $(nfmv_O) nfmv.ld $(nfmv_TU) $(if $(nfmv_TU),$(shell find include -name '*.h') tools/linkcheck.py tools/fnscore.py)
	$(if $(nfmv_TU),$(PYTHON) tools/linkcheck.py --target nfmv -j 2 --workdir $(BUILD)/tulink-nfmv --link $@ --tu $(nfmv_TU),$(LD) -EL -T undefined_syms_auto.nfmv.txt -T undefined_funcs_auto.nfmv.txt -T nfmv.ld -Map $(BUILD)/nfmv.map --no-check-sections -o $@)
$(BUILD)/NFMV.EXE: $(BUILD)/nfmv.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CFLAGS) | $(MASPSX) | $(AS) $(ASFLAGS) -o $@

ifeq ($(strip $(TU_FILES)),)
$(BUILD)/$(BASENAME).elf: $(O_FILES) $(BASENAME).ld
	$(LD) -EL -T undefined_syms_auto.txt -T undefined_syms_manual.txt -T undefined_funcs_auto.txt -T $(BASENAME).ld -Map $(BUILD)/$(BASENAME).map --no-check-sections -o $@
else
$(BUILD)/$(BASENAME).elf: $(O_FILES) $(BASENAME).ld $(TU_FILES) $(shell find include -name '*.h') tools/linkcheck.py tools/fnscore.py
	$(PYTHON) tools/linkcheck.py -j 2 --workdir $(BUILD)/tulink --link $@ --tu $(TU_FILES)
endif

# .sbss starts at 0x96FBC, inside the file's 2 KB zero padding (0x877BC-0x877FF): pad back to 0x87800
$(BUILD)/SLUS_008.42: $(BUILD)/$(BASENAME).elf
	$(OBJCOPY) -O binary --pad-to=0x87800 $< $@

clean:
	rm -rf $(BUILD)
.PHONY: all clean overlays $(OVERLAYS) nfmv
