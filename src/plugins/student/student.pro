#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    fibonacci.h \
    jump.h \
    logistic_growth.h
	
SOURCES +=  \
    fibonacci.cpp \
    jump.cpp \
    logistic_growth.cpp

