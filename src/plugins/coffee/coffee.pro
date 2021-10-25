#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = coffee

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
above_ground.h \
coffee_documentation.h \
phenology.h \

SOURCES +=  \
above_ground.cpp \
coffee_documentation.cpp \
phenology.cpp \

