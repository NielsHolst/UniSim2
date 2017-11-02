#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    day_length_scale.h \
    fibonacci.h \
    jump.h
    
SOURCES +=  \
    day_length_scale.cpp \
    fibonacci.cpp \
    jump.cpp


