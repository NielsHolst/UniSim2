#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = farm

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    fibonacci.h \
    goat.h
	
SOURCES +=  \
    fibonacci.cpp \
    goat.cpp


