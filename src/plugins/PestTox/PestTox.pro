#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = PestTox

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS += \
    application.h \
    crop.h \
    eggs_laid.h \
    field_area.h \
    fraction_of_rain_run_off.h \
    leaf_photo_degradation.h \
    leaf_overall_rate_constant.h \
    leaf_uptake.h \
    leaf_volatilization.h \
    leaf_wash_off.h \
    macropore_flow.h \
    organic_carbon_water_partitioning.h \
    pesticide_induced_mortality_rate.h \
    primary_distribution_drift_to_air.h \
    primary_distribution_leaf.h \
    primary_distribution_soil.h \
    run_off_amount.h \
    soil_degradation_rate_constant.h \
    soil_porosity.h \
    surface_water_degradation_final.h \
    topsoil_degradation_final.h \
    topsoil_dose_in_soil_solids_air_and_water.h \
    topsoil_volatilization.h \
    topsoil_run_off.h \
    viscosity_of_water.h \
    water_degradation_rate_constant.h \
    zvalues.h

SOURCES += \
    application.cpp \
    crop.cpp \
    eggs_laid.cpp \
    field_area.cpp \
    fraction_of_rain_run_off.cpp \
    leaf_photo_degradation.cpp \
    leaf_overall_rate_constant.cpp \
    leaf_uptake.cpp \
    leaf_volatilization.cpp \
    leaf_wash_off.cpp \
    macropore_flow.cpp \
    organic_carbon_water_partitioning.cpp \
    pesticide_induced_mortality_rate.cpp \
    primary_distribution_drift_to_air.cpp \
    primary_distribution_leaf.cpp \
    primary_distribution_soil.cpp \
    run_off_amount.cpp \
    soil_degradation_rate_constant.cpp \
    soil_porosity.cpp \
    surface_water_degradation_final.cpp \
    topsoil_degradation_final.cpp \
    topsoil_dose_in_soil_solids_air_and_water.cpp \
    topsoil_volatilization.cpp \
    topsoil_run_off.cpp \
    viscosity_of_water.cpp \
    water_degradation_rate_constant.cpp \
    zvalues.cpp
