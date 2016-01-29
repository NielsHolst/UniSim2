#-------------------------------------------------
#
# command DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = command

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    cd.h \
    clear.h \
    list.h \
    set.h \
    quit.h
	
SOURCES +=  \
    cd.cpp \
    clear.cpp \
    list.cpp \
    set.cpp \
    quit.cpp


