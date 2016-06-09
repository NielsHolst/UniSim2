#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = apis

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    life_stage.h \
    nectar_demand.h \
    nectar_flow.h
	
SOURCES +=  \
    life_stage.cpp \
    nectar_demand.cpp \
    nectar_flow.cpp


