#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = conductance

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS += \
    phase.h \
    plant.h

SOURCES += \
    plant.cpp
