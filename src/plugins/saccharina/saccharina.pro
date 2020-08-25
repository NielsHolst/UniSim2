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
    Dryweight.h \
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
    Salinity.h \
    Seasonal_influence_on_growthrate.h \
    Specific_growth_rate.h \
    Structural_dryweight.h \
    Temperature_dependent_respiration.h \
    Total_carbon.h \
    Total_nitrogen.h \
    Wetweight.h \
    Yieldprm.h \
    light_inhibition.h
    
SOURCES +=  \
    Area.cpp \
    Area_lost.cpp \
    Beta.cpp \
    Carbon_content.cpp \
    Carbon_exudation.cpp \
    Dryweight.cpp \
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
    Salinity.cpp \
    Seasonal_influence_on_growthrate.cpp \
    Specific_growth_rate.cpp \
    Structural_dryweight.cpp \
    Temperature_dependent_respiration.cpp \
    Total_carbon.cpp \
    Total_nitrogen.cpp \
    Wetweight.cpp \
    Yieldprm.cpp \
    light_inhibition.cpp


