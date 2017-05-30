#-------------------------------------------------
#
# command DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = distribution

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")


HEADERS +=  \
    lognormal.h \
    log_parameter.h \
    loguniform.h \
    normal.h \
    uniform.h
	
SOURCES +=  \
    lognormal.cpp \
    log_parameter.cpp \
    loguniform.cpp \
    normal.cpp \
    uniform.cpp


