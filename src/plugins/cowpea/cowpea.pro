#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = cowpea

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    biomass.h \
    cowpea_population.h \
    degrees_to_days.h \
    exponential_growth.h \
    food_web_population.h \
    in_grams.h \
    in_grams2.h \
    in_larvae.h \
    increment.h \
    initial_biomass.h \
    li.h \
    rgr.h \
    single_biomass.h \
    threshold.h 
    
SOURCES +=  \
    biomass.cpp \
    cowpea_population.cpp \
    degrees_to_days.cpp \
    exponential_growth.cpp \
    food_web_population.cpp \
    in_grams.cpp \
    in_grams2.cpp \
    in_larvae.cpp \
    increment.cpp \
    initial_biomass.cpp \
    li.cpp \
    rgr.cpp \
    single_biomass.cpp \
    threshold.cpp 


