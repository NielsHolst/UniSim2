#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = saccharina

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    area.h \
    area_loss_erosion.h \
    biomass.h \
    carbon_reserve.h \
    exudation.h \
    f_area.h \
    f_carbon.h \
    f_nitrogen.h \
    f_salinity.h \
    f_temperature.h \
    gross_photosynthesis.h \
    growth_rate.h \
    light_inhibition.h \
    nitrogen_reserve.h \
    nitrogen_uptake_rate.h \
    pmax.h \
    respiration.h
    
SOURCES +=  \
    area.cpp \
    area_loss_erosion.cpp \
    biomass.cpp \
    carbon_reserve.cpp \
    exudation.cpp \
    f_area.cpp \
    f_carbon.cpp \
    f_nitrogen.cpp \
    f_salinity.cpp \
    f_temperature.cpp \
    gross_photosynthesis.cpp \
    growth_rate.cpp \
    light_inhibition.cpp \
    nitrogen_reserve.cpp \
    nitrogen_uptake_rate.cpp \
    pmax.cpp \
    respiration.cpp
