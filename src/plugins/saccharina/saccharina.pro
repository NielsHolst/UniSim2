#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = saccharina

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    Area.h \
    Area_lost.h \
    Beta.h \
    Carbon_content.h \
    Carbon_exudation.h \
    Effect_of_size_on_growthrate.h \
    Effect_of_temperature_on_growthrate.h \
    Frond_erosion.h \
    Gross_photosynthesis.h \
    Maximal_photosynthesis_rate_reftemp.h \
    Maximal_photosynthetic_rate.h \
    Nitrate_uptake_rate.h \
    Nitrogen_content.h \
    Photosynthesis.h \
    Rate_of_change_in_carbon_reserves.h \
    Rate_of_change_in_nitrogen_reserves.h \
    Rate_of_change_of_frond_area.h \
    Seasonal_influence_on_growthrate.h \
    Specific_growth_rate.h \
    Temperature_dependent_respiration.h \
    light_inhibition.h
    
SOURCES +=  \
    Area.cpp \
    Area_lost.cpp \
    Beta.cpp \
    Carbon_content.cpp \
    Carbon_exudation.cpp \
    Effect_of_size_on_growthrate.cpp \
    Effect_of_temperature_on_growthrate.cpp \
    Frond_erosion.cpp \
    Gross_photosynthesis.cpp \
    Maximal_photosynthesis_rate_reftemp.cpp \
    Maximal_photosynthetic_rate.cpp \
    Nitrate_uptake_rate.cpp \
    Nitrogen_content.cpp \
    Photosynthesis.cpp \
    Rate_of_change_in_carbon_reserves.cpp \
    Rate_of_change_in_nitrogen_reserves.cpp \
    Rate_of_change_of_frond_area.cpp \
    Seasonal_influence_on_growthrate.cpp \
    Specific_growth_rate.cpp \
    Temperature_dependent_respiration.cpp \
    light_inhibition.cpp


