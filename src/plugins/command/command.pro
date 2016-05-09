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
    help.h \
    list.h \
    load.h \
    profile.h \
    run.h \
    save.h \
    set.h \
    set_font.h \
    set_input_folder.h \
    set_output_folder.h \
    set_script_folder.h \
    test.h \
    update.h \
    quit.h
	
SOURCES +=  \
    cd.cpp \
    clear.cpp \
    help.cpp \
    list.cpp \
    load.cpp \
    profile.cpp \
    run.cpp \
    save.cpp \
    set.cpp \
    set_font.cpp \
    set_input_folder.cpp \
    set_output_folder.cpp \
    set_script_folder.cpp \
    test.cpp \
    update.cpp \
    quit.cpp


