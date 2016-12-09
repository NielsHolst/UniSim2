#-------------------------------------------------
#
# command DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = distribution

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")


HEADERS +=  \
    even.h \
    normal.h
	
SOURCES +=  \
    even.cpp \
    normal.cpp


