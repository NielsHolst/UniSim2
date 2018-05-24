#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = bmsb

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    soep_mort_rate.h \
    ssi_dev_rate.h
    
SOURCES +=  \
    soep_mort_rate.cpp \
    ssi_dev_rate.cpp


