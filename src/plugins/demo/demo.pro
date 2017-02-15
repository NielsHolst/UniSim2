#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = demo

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    island.h \
    migration.h \
    population.h
	
SOURCES +=  \
    island.cpp \
    migration.cpp \
    population.cpp

