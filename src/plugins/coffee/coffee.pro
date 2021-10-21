#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = coffee

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    coffee_documentation.h \
    phenology.h \

SOURCES +=  \
    coffee_documentation.cpp \
    phenology.cpp \

