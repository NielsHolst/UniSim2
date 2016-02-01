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
    run.h \
    save.h \
    save_output.h \
    set.h \
    set_font.h \
    set_output_folder.h \
    test.h \
    update.h \
    quit.h
	
SOURCES +=  \
    cd.cpp \
    clear.cpp \
    list.cpp \
    run.cpp \
    save.cpp \
    save_output.cpp \
    set.cpp \
    set_font.cpp \
    set_output_folder.cpp \
    test.cpp \
    update.cpp \
    quit.cpp


