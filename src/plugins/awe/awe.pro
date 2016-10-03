#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = awe

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    crop.h \
#    instant_mortality.h \
    rotation.h\
#    seed_bank.h \
    weather.h
#    weed.h
	
SOURCES +=  \
    crop.cpp \
#    instant_mortality.cpp \
    rotation.cpp\
#    seed_bank.cpp \
    weather.cpp
#    weed.cpp
