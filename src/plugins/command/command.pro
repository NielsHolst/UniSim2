#-------------------------------------------------
#
# command DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = command

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    batch.h \
    clear.h \
    clip.h \
    debug.h \
    doc.h \
    edit.h \
    find.h \
    general.h \
    go.h \
#    graph.h \
    help.h \
    help_class.h \
    list.h \
    list_options.h \
    list_output.h \
    load.h \
    profile.h \
    reconfigure.h \
    reset.h \
    run.h \
    save.h \
    set.h \
    set_font.h \
    set_folder.h \
#    test.h \
#    update.h \
    what.h \
    write.h \
    quit.h
	
SOURCES +=  \
    batch.cpp \
    clear.cpp \
    clip.cpp \
    debug.cpp \
    doc.cpp \
    edit.cpp \
    find.cpp \
    general.cpp \
    go.cpp \
#    graph.cpp \
    help.cpp \
    help_class.cpp \
    list.cpp \
    list_options.cpp \
    list_output.cpp \
    load.cpp \
    profile.cpp \
    reconfigure.cpp \
    reset.cpp \
    run.cpp \
    save.cpp \
    set.cpp \
    set_font.cpp \
    set_folder.cpp \
#    test.cpp \
#    update.cpp \
    what.cpp \
    write.cpp \
    quit.cpp


