#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX, Christopher Arndt, and Patrick Desaulniers
#

# --------------------------------------------------------------
# Installation directories

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
DSSI_DIR ?= $(LIBDIR)/dssi
LADSPA_DIR ?= $(LIBDIR)/ladspa
ifneq ($(MACOS_OR_WINDOWS),true)
LV2_DIR ?= $(LIBDIR)/lv2
VST_DIR ?= $(LIBDIR)/vst
endif
ifeq ($(MACOS),true)
LV2_DIR ?= /Library/Audio/Plug-Ins/LV2
VST_DIR ?= /Library/Audio/Plug-Ins/VST
endif
ifeq ($(WINDOWS),true)
LV2_DIR ?= $(COMMONPROGRAMFILES)/LV2
VST_DIR ?= $(COMMONPROGRAMFILES)/VST2
endif

USER_DSSI_DIR ?= $(HOME)/.dssi
USER_LADSPA_DIR ?= $(HOME)/.ladspa
ifneq ($(MACOS_OR_WINDOWS),true)
USER_LV2_DIR ?= $(HOME)/.lv2
USER_VST_DIR ?= $(HOME)/.vst
endif
ifeq ($(MACOS),true)
USER_LV2_DIR ?= $(HOME)/Library/Audio/Plug-Ins/LV2
USER_VST_DIR ?= $(HOME)/Library/Audio/Plug-Ins/VST
endif
ifeq ($(WINDOWS),true)
USER_LV2_DIR ?= $(APPDATA)/LV2
USER_VST_DIR ?= $(APPDATA)/VST
endif

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ensemble-chorus

# --------------------------------------------------------------
# Plugin types to build

BUILD_LV2 ?= true
BUILD_VST2 ?= true
BUILD_JACK ?= true
BUILD_DSSI ?= false
BUILD_LADSPA ?= false

# --------------------------------------------------------------
# Files to build

FILES_SHARED = \
  EnsembleChorusShared.cpp \
  sources/core/bbd.cc \
  sources/core/chorus.cc \
  sources/core/lfos.cc \
  sources/core/core.cc \
  # sources/core/core_preset.cc

FILES_SHARED += \
  thirdparty/DSPFilters/shared/DSPFilters/source/Biquad.cpp \
  thirdparty/DSPFilters/shared/DSPFilters/source/Butterworth.cpp \
  thirdparty/DSPFilters/shared/DSPFilters/source/Cascade.cpp \
  thirdparty/DSPFilters/shared/DSPFilters/source/Filter.cpp \
  thirdparty/DSPFilters/shared/DSPFilters/source/Param.cpp \
  thirdparty/DSPFilters/shared/DSPFilters/source/PoleFilter.cpp \
  thirdparty/DSPFilters/shared/DSPFilters/source/RBJ.cpp \
  thirdparty/DSPFilters/shared/DSPFilters/source/RootFinder.cpp

FILES_DSP = \
  $(FILES_SHARED) \
  PluginEnsembleChorus.cpp

FILES_UI = \
  $(FILES_SHARED) \
  UIEnsembleChorus.cpp \
  Artwork.cpp \
  sources/ui/Cairo++.cpp \
  sources/ui/FontEngine.cpp \
  sources/ui/Fontstash.cpp \
  sources/ui/components/BasicWidget.cpp \
  sources/ui/components/LookAndFeel.cpp \
  sources/ui/components/Slider.cpp \
  sources/ui/components/TextLabel.cpp \
  sources/ui/components/ToggleButton.cpp \
  sources/ui/components/ToggleButtonGroup.cpp \
  sources/ui/ensemble-chorus/ModulatorView.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = cairo
include ../../dpf/Makefile.plugins.mk

BUILD_CXX_FLAGS += $(shell $(PKG_CONFIG) --cflags zlib)
LINK_FLAGS += $(shell $(PKG_CONFIG) --libs zlib)

BUILD_CXX_FLAGS += \
  -Isources -Isources/core/include -Imeta \
  -Ithirdparty/DSPFilters/shared/DSPFilters/include \
  -Ithirdparty/blink \
  -Ithirdparty/jsl/include \
  -Ithirdparty/gsl-lite/include \
  -Ithirdparty/fontstash \
  -Ithirdparty/stb \
  -Ithirdparty/gzip-hpp/include \
  -DEC_API=

# --------------------------------------------------------------
# Enable all selected plugin types

ifeq ($(BUILD_LV2),true)
ifeq ($(HAVE_CAIRO),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif
endif

ifeq ($(BUILD_VST2),true)
TARGETS += vst
endif

ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(BUILD_DSSI),true)
ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_CAIRO),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
endif
endif
endif

ifeq ($(BUILD_LADSPA),true)
TARGETS += ladspa
endif

all: $(TARGETS)

install: all
ifeq ($(BUILD_DSSI),true)
ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_CAIRO),true)
ifeq ($(HAVE_LIBLO),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)-dssi$(LIB_EXT) $(DESTDIR)$(DSSI_DIR)/$(NAME)-dssi$(LIB_EXT)
endif
endif
endif
endif
ifeq ($(BUILD_LADSPA),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)-ladspa$(LIB_EXT) $(DESTDIR)$(LADSPA_DIR)/$(NAME)-ladspa$(LIB_EXT)
endif
ifeq ($(BUILD_VST2),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT) $(DESTDIR)$(VST_DIR)/$(NAME)-vst$(LIB_EXT)
endif
ifeq ($(BUILD_LV2),true)
	@install -dm755 $(DESTDIR)$(LV2_DIR)/$(NAME).lv2 && \
		install -m644 $(TARGET_DIR)/$(NAME).lv2/*.ttl $(DESTDIR)$(LV2_DIR)/$(NAME).lv2 && \
		install -m755 $(TARGET_DIR)/$(NAME).lv2/*.so $(DESTDIR)$(LV2_DIR)/$(NAME).lv2
endif
ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)$(APP_EXT) $(DESTDIR)$(BINDIR)/$(NAME)$(APP_EXT)
endif
endif

install-user: all
ifeq ($(BUILD_DSSI),true)
ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_CAIRO),true)
ifeq ($(HAVE_LIBLO),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)-dssi$(LIB_EXT) $(USER_DSSI_DIR)/$(NAME)-dssi$(LIB_EXT)
endif
endif
endif
endif
ifeq ($(BUILD_LADSPA),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)-ladspa$(LIB_EXT) $(USER_LADSPA_DIR)/$(NAME)-ladspa$(LIB_EXT)
endif
ifeq ($(BUILD_VST2),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT) $(USER_VST_DIR)/$(NAME)-vst$(LIB_EXT)
endif
ifeq ($(BUILD_LV2),true)
	@install -dm755 $(USER_LV2_DIR)/$(NAME).lv2 && \
		install -m644 $(TARGET_DIR)/$(NAME).lv2/*.ttl $(USER_LV2_DIR)/$(NAME).lv2 && \
		install -m755 $(TARGET_DIR)/$(NAME).lv2/*.so $(USER_LV2_DIR)/$(NAME).lv2
endif
ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
	@install -Dm755 $(TARGET_DIR)/$(NAME)$(APP_EXT) $(HOME)/bin/$(NAME)$(APP_EXT)
endif
endif

# --------------------------------------------------------------

.PHONY: all install install-user

# --------------------------------------------------------------

layouts: gen/EnsembleChorusUILayouts.Main.inc
.PHONY: layouts

LAYOUT_TOOL := ../../tools/layout-tool/bin/layout-tool$(APP_EXT)
$(LAYOUT_TOOL):
	$(MAKE) -C ../../tools/layout-tool

gen/EnsembleChorusUILayouts.%.inc: layouts/%.fl $(LAYOUT_TOOL)
	@mkdir -p gen
	$(LAYOUT_TOOL) $< > $@

# --------------------------------------------------------------

RES2C := ../../dpf/utils/res2c.py
artwork:
	$(RES2C) Artwork resources/ui/artwork
.PHONY: artwork
