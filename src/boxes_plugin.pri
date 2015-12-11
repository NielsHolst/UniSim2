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
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin/plugins"
TARGET = $$BOXES_PLUGIN_NAME$${SUFFIX}
DEFINES += "BOXES_PLUGIN_NAME=$$BOXES_PLUGIN_NAME"
#CONFIG += dll ??

# Own library source code on which we depend
DEPENDPATH += $$_PRO_FILE_PWD_/../boxes

# Location of library when installed on Unix
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Additional folders with header files
INCLUDEPATH += "$$_PRO_FILE_PWD_/.."

# Common pluging headers and soruces
HEADERS +=  \
    ../boxes/publish.h \
    ../boxes/factory.h

SOURCES +=  \
    ../boxes/publish.cpp \
    ../boxes/factory.cpp



