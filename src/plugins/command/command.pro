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
    find.h \
    general.h \
    go.h \
    graph.h \
    help.h \
    help_class.h \
    list.h \
    load.h \
    profile.h \
    reconfigure.h \
    reset.h \
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
    find.cpp \
    general.cpp \
    go.cpp \
    graph.cpp \
    help.cpp \
    help_class.cpp \
    list.cpp \
    load.cpp \
    profile.cpp \
    reconfigure.cpp \
    reset.cpp \
    run.cpp \
    save.cpp \
    set.cpp \
    set_font.cpp \
    set_folder.cpp \
    test.cpp \
    update.cpp \
    quit.cpp


