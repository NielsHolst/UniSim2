#-------------------------------------------------
#
# Project to include in all plugin projects
# 
# Usage:
# BOXES_PLUGIN_NAME = savanna
# include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")
# HEADERS += ...
# SOURCES += ...
# 
#-------------------------------------------------

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

# What we are building
TEMPLATE = lib
TARGET = $$BOXES_PLUGIN_NAME$${SUFFIX}
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin/plugins"
DEFINES += BOXES_PLUGIN_NAME=$$BOXES_PLUGIN_NAME
DEFINES += BOXES_PLUGIN_NAME_TEXT=\\\"$${BOXES_PLUGIN_NAME}\\\"

# Plugin destination folder set from environment or not
#UNISIM_PLUGINS_VAR = $$(UNISIM_PLUGINS)
#isEmpty(UNISIM_PLUGINS_VAR) {
#    DESTDIR = "$$_PRO_FILE_PWD_/../../../bin/plugins"
#    message("Using local plugins destination")
#}
#!isEmpty(UNISIM_PLUGINS_VAR) {
#    DESTDIR= "$$(UNISIM_PLUGINS)"
#    message($$join(UNISIM_PLUGINS_VAR, " ", "Including "))
#}

# Own library source code on which we depend
DEPENDPATH += $$_PRO_FILE_PWD_/../base

# Location of library when installed on Unix
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Additional folders with header files
INCLUDEPATH += "$$_PRO_FILE_PWD_/.."

# Common plugin headers and sources
HEADERS +=  \
    ../base/publish.h \
    ../base/factory.h

SOURCES +=  \
    ../base/publish.cpp \
    ../base/factory.cpp



