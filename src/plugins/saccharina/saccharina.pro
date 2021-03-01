#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = saccharina

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    allocation.h \
    area.h \
    area_erosion.h \
    biomass.h \
    calibration.h \
    demand_carbon_erosion.h \
    demand_carbon_exudation.h \
    demand_carbon_reserves.h \
    demand_carbon_respiration.h \
    demand_carbon_structure.h \
    demand_nitrogen_erosion.h \
    demand_nitrogen_reserves.h \
    demand_nitrogen_structure.h \
    f_area.h \
    f_current.h \
    f_nitrogen.h \
    f_salinity.h \
    nitrogen_uptake.h \
    photosynthesis.h \
    reserves.h \
    saccharina_documentation.h \
    structure.h \
    trapezoid.h
    
SOURCES +=  \
    allocation.cpp \
    area.cpp \
    area_erosion.cpp \
    biomass.cpp \
    calibration.cpp \
    demand_carbon_erosion.cpp \
    demand_carbon_exudation.cpp \
    demand_carbon_reserves.cpp \
    demand_carbon_respiration.cpp \
    demand_carbon_structure.cpp \
    demand_nitrogen_erosion.cpp \
    demand_nitrogen_reserves.cpp \
    demand_nitrogen_structure.cpp \
    f_area.cpp \
    f_current.cpp \
    f_nitrogen.cpp \
    f_salinity.cpp \
    nitrogen_uptake.cpp \
    photosynthesis.cpp \
    reserves.cpp \
    saccharina_documentation.cpp \
    structure.cpp \
    trapezoid.cpp
