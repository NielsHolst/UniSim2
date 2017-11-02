#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = eq

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    exponential.h \
    gompertz.h \
    inv_log10.h \
    log10.h \
    transition.h
	
SOURCES +=  \
    exponential.cpp \
    gompertz.cpp \
    inv_log10.cpp \
    log10.cpp \
    transition.cpp


