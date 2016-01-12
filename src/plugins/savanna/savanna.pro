#-------------------------------------------------
#
# savanna DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = savanna

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    grazer.h \
    lion.h
	
SOURCES +=  \
    grazer.cpp \
    lion.cpp


