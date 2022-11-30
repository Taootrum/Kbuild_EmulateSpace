# ===========================================================================
# Kernel configuration targets
# These targets are used from top-level makefile

PHONY += oldconfig menuconfig config silentoldconfig

ifdef KBUILD_KCONFIG
Kconfig := $(KBUILD_KCONFIG)
else
Kconfig := Kconfig
endif

menuconfig: $(obj)/mconf
	$< $(Kconfig)

config: $(obj)/conf
	$< --oldaskconfig $(Kconfig)

oldconfig: $(obj)/conf
	$< --$@ $(Kconfig)

silentoldconfig: $(obj)/conf
	$(Q)mkdir -p include/generated
	$< --$@ $(Kconfig)

PHONY += allnoconfig allyesconfig allmodconfig alldefconfig randconfig

allnoconfig allyesconfig allmodconfig alldefconfig randconfig: $(obj)/conf
	$< --$@ $(Kconfig)

PHONY += listnewconfig oldnoconfig savedefconfig defconfig

listnewconfig oldnoconfig: $(obj)/conf
	$< --$@ $(Kconfig)

savedefconfig: $(obj)/conf
	$< --$@=$(srctree)/configs/defconfig $(Kconfig)

# ##$(info KBUILD_DEFCONFIG=$(KBUILD_DEFCONFIG))
defconfig: $(obj)/conf
ifeq ($(KBUILD_DEFCONFIG),)
	$< --defconfig $(Kconfig)
else
	@echo "*** Default configuration is based on '$(KBUILD_DEFCONFIG)'"
# location of configs changed by Masahiro Yamada
	$(Q)$< --defconfig=configs/$(KBUILD_DEFCONFIG) $(Kconfig)
endif

%_defconfig: $(obj)/conf
# location of configs changed by Masahiro Yamada
	$(Q)$< --defconfig=configs/$@ $(Kconfig)

# Help text used by make help
help:	
	@echo  '  config	  - Update current config utilising a line-oriented program'
	@echo  '  menuconfig	  - Update current config utilising a menu based program'
	@echo  '  oldconfig	  - Update current config utilising a provided .config as base'
	@echo  '  silentoldconfig - Same as oldconfig, but quietly, additionally update deps'
	@echo  '  defconfig	  - New config with default from ARCH supplied defconfig'
	@echo  '  savedefconfig   - Save current config as ./defconfig (minimal config)'
	@echo  '  allnoconfig	  - New config where all options are answered with no'
	@echo  '  allyesconfig	  - New config where all options are accepted with yes'
	@echo  '  allmodconfig	  - New config selecting modules when possible'
	@echo  '  alldefconfig    - New config with all symbols set to default'
	@echo  '  randconfig	  - New config with random answer to all options'
	@echo  '  listnewconfig   - List new options'
	@echo  '  oldnoconfig     - Same as silentoldconfig but set new symbols to n (unset)'

# lxdialog stuff
check-lxdialog  := $(srctree)/$(src)/lxdialog/check-lxdialog.sh

# Use recursively expanded variables so we do not call gcc unless
# we really need to do so. (Do not call gcc as part of make mrproper)
HOST_EXTRACFLAGS += $(shell $(CONFIG_SHELL) $(check-lxdialog) -ccflags) \
                    -DLOCALE

# ===========================================================================
# Shared Makefile for the various kconfig executables:
# conf:	  Used for defconfig, oldconfig and related targets
# mconf:  Used for the menuconfig target
#         Utilizes the lxdialog package
# object files used by all kconfig flavours

lxdialog := lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o
lxdialog += lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o

conf-objs	:= conf.o  zconf.tab.o
mconf-objs     := mconf.o zconf.tab.o $(lxdialog)
kxgettext-objs	:= kxgettext.o zconf.tab.o

hostprogs-y := conf

ifeq ($(MAKECMDGOALS),menuconfig)
	hostprogs-y += mconf
endif

clean-files	:= lkc_defs.h qconf.moc .tmp_qtcheck .tmp_gtkcheck
clean-files	+= zconf.tab.c lex.zconf.c zconf.hash.c gconf.glade.h
clean-files     += mconf qconf gconf nconf
clean-files     += config.pot linux.pot

# Check that we have the required ncurses stuff installed for lxdialog (menuconfig)
PHONY += $(obj)/dochecklxdialog
$(addprefix $(obj)/,$(lxdialog)): $(obj)/dochecklxdialog
$(obj)/dochecklxdialog:
	$(Q)$(CONFIG_SHELL) $(check-lxdialog) -check $(HOSTCC) $(HOST_EXTRACFLAGS) $(HOSTLOADLIBES_mconf)

always := dochecklxdialog

# Add environment specific flags
HOST_EXTRACFLAGS += $(shell $(CONFIG_SHELL) $(srctree)/$(src)/check.sh $(HOSTCC) $(HOSTCFLAGS))

# generated files seem to need this to find local include files
HOSTCFLAGS_lex.zconf.o	:= -I$(src)
HOSTCFLAGS_zconf.tab.o	:= -I$(src)

HOSTLOADLIBES_mconf   = $(shell $(CONFIG_SHELL) $(check-lxdialog) -ldflags $(HOSTCC))

$(obj)/zconf.tab.o: $(obj)/lex.zconf.c $(obj)/zconf.hash.c

$(obj)/kconfig_load.o: $(obj)/lkc_defs.h

$(obj)/%.moc: $(src)/%.h
	$(KC_QT_MOC) -i $< -o $@

$(obj)/lkc_defs.h: $(src)/lkc_proto.h
	$(Q)sed < $< > $@ 's/P(\([^,]*\),.*/#define \1 (\*\1_p)/'