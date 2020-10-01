#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = test

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    area.h \
    data_frame_input.h \
    model_a.h \
    vector_input.h
	
SOURCES +=  \
    area.cpp \
    data_frame_input.cpp \
    model_a.cpp\
    vector_input.cpp


