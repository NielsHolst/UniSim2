#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = test_boxes

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    data_frame_input.h \
    model_a.h \
    vector_input.h
	
SOURCES +=  \
    data_frame_input.cpp \
    model_a.cpp\
    vector_input.cpp


