#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = test_boxes

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    model_a.h \
    vector_input.h
	
SOURCES +=  \
    model_a.cpp\
    vector_input.cpp


