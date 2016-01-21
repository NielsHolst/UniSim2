#-------------------------------------------------
#
# command DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = command

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    command.h \
    setwd.h
	
SOURCES +=  \
    command.cpp \
    setwd.cpp


