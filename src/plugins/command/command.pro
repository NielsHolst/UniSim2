#-------------------------------------------------
#
# command DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = command

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    clear.h \
    graph.h \
    help.h \
    list.h \
    load.h \
    profile.h \
    run.h \
    save.h \
    set.h \
    set_font.h \
    set_folder.h \
    test.h \
    update.h \
    quit.h
	
SOURCES +=  \
    clear.cpp \
    graph.cpp \
    help.cpp \
    list.cpp \
    load.cpp \
    profile.cpp \
    run.cpp \
    save.cpp \
    set.cpp \
    set_font.cpp \
    set_folder.cpp \
    test.cpp \
    update.cpp \
    quit.cpp


