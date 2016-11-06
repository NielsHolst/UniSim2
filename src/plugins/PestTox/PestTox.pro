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
    fraction_of_rain_run_off.h \
    general.h \
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
    soil_porosity.h \
    soil_temperature.h \
    surface_water_degradation.h \
    topsoil_degradation.h \
    topsoil_in_soil.h \
    topsoil_volatilization.h \
    topsoil_run_off.h \
    water_viscosity.h \
    zvalues.h

SOURCES += \
    application.cpp \
    crop.cpp \
    eggs_laid.cpp \
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
    soil_porosity.cpp \
    soil_temperature.cpp \
    surface_water_degradation.cpp \
    topsoil_degradation.cpp \
    topsoil_in_soil.cpp \
    topsoil_volatilization.cpp \
    topsoil_run_off.cpp \
    water_viscosity.cpp \
    zvalues.cpp
