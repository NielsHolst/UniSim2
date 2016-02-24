#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = vg

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    air_flux_cooling_supply.h \
    air_flux_cooling_supply_max.h \
    air_flux_given.h \
    air_flux_gravitation.h \
    air_flux_infiltration.h \
    base_control_element.h \
    base_signal.h \
    boundary_layer_resistance_base.h \
    boundary_layer_resistance_jones_a.h \
    boundary_layer_resistance_jones_b.h \
    boundary_layer_resistance_stanghellini.h \
    boundary_layer_resistance_monteith_unsworth.h \
    budget.h \
    co2_controller.h \
    construction_geometry.h \
    cover.h \
#    crop.h \
#    crop_mass.h \
#    crop_growth.h \
#    crop_lai.h \
#    crop_radiation.h \
#    crop_yield.h \
    data_grid.h \
#    date_time_signal.h \
#    energy_flux_air.h \
#    energy_flux_base.h \
#    energy_flux_condensation.h \
#    energy_flux_cooling_demand.h \
#    energy_flux_cooling_supply.h \
#    energy_flux_floor.h \
#    energy_flux_growth_lights.h \
#    energy_flux_heating_demand.h \
#    energy_flux_shelters.h \
#    energy_flux_sum.h \
#    energy_flux_transpiration.h \
#    energy_screen_balance_signal.h \
#    fixed_signal.h \
#    floor_radiation_absorbed.h \
#    fruit_crop_lai.h \
#    fruit_crop_mass.h \
#    fruit_factor.h \
    general.h \
#    growth_light_base.h \
#    growth_light_controller.h \
#    growth_light.h \
#    growth_lights.h \
#    indoors_co2.h \
#    indoors_humidity.h \
#    indoors_light.h \
#    indoors_temperature.h \
#    indoors_wind_speed.h \
    interpolate.h \
#    leaf_layer.h \
#    leaf_light_response.h \
#    leaf_photosynthesis.h \
#    leaf_radiation_absorbed.h \
#    leaf_temperature.h \
#    leaf_transpiration.h \
#    leaf_wind_speed.h \
#    outdoors.h \
#    pid_control_element.h \
#    pipe.h \
#    proportional_signal.h \
#    running_average.h \
#    screen.h \
#    screens.h \
#    shelter.h \
#    shelter_base.h \
#    shelters.h \
#    signal_collection.h \
#    sliding_signal.h \
#    stomatal_resistance_base.h \
#    stomatal_resistance_rose.h \
#    stomatal_resistance_tomato.h \
    surface_radiation.h \
    surface_radiation_outputs.h \
#    threshold_signal.h \
    utilities.h \
#    vapour_flux_air.h \
#    vapour_flux_base.h \
#    vapour_flux_condensation.h \
#    vapour_flux_sum.h \
#    vapour_flux_sum_base.h \
#    vapour_flux_transpiration.h \
#    ventilated_latent_heat_converter.h \
#    ventilation_by_temp.h \
#    ventilation_by_wind.h \
#    vent.h \
#    vents.h

SOURCES += \
    air_flux_cooling_supply.cpp \
    air_flux_cooling_supply_max.cpp \
    air_flux_given.cpp \
    air_flux_gravitation.cpp \
    air_flux_infiltration.cpp \
    base_control_element.cpp \
    base_signal.cpp \
    boundary_layer_resistance_base.cpp \
    boundary_layer_resistance_jones_a.cpp \
    boundary_layer_resistance_jones_b.cpp \
    boundary_layer_resistance_stanghellini.cpp \
    boundary_layer_resistance_monteith_unsworth.cpp \
    budget.cpp \
    co2_controller.cpp \
    construction_geometry.cpp \
    cover.cpp \
#    crop.cpp \
#    crop_mass.cpp \
#    crop_growth.cpp \
#    crop_lai.cpp \
#    crop_radiation.cpp \
#    crop_yield.cpp \
    data_grid.cpp \
#    date_time_signal.cpp \
#    energy_flux_air.cpp \
#    energy_flux_base.cpp \
#    energy_flux_condensation.cpp \
#    energy_flux_cooling_demand.cpp \
#    energy_flux_cooling_supply.cpp \
#    energy_flux_floor.cpp \
#    energy_flux_growth_lights.cpp \
#    energy_flux_heating_demand.cpp \
#    energy_flux_shelters.cpp \
#    energy_flux_sum.cpp \
#    energy_flux_transpiration.cpp \
#    energy_screen_balance_signal.cpp \
#    fixed_signal.cpp \
#    floor_radiation_absorbed.cpp \
#    fruit_crop_lai.cpp \
#    fruit_crop_mass.cpp \
#    fruit_factor.cpp \
    general.cpp \
#    growth_light_base.cpp \
#    growth_light_controller.cpp \
#    growth_light.cpp \
#    growth_lights.cpp \
#    indoors_co2.cpp \
#    indoors_humidity.cpp \
#    indoors_light.cpp \
#    indoors_temperature.cpp \
#    indoors_wind_speed.cpp \
#    leaf_layer.cpp \
#    leaf_light_response.cpp \
#    leaf_photosynthesis.cpp \
#    leaf_radiation_absorbed.cpp \
#    leaf_temperature.cpp \
#    leaf_transpiration.cpp \
#    leaf_wind_speed.cpp \
#    outdoors.cpp \
#    pid_control_element.cpp \
#    pipe.cpp \
#    proportional_signal.cpp \
#    running_average.cpp \
#    screen.cpp \
#    screens.cpp \
#    shelter.cpp \
#    shelter_base.cpp \
#    shelters.cpp \
#    signal_collection.cpp \
#    sliding_signal.cpp \
#    stomatal_resistance_base.cpp \
#    stomatal_resistance_rose.cpp \
#    stomatal_resistance_tomato.cpp \
    surface_radiation.cpp \
    surface_radiation_outputs.cpp \
#    threshold_signal.cpp \
    utilities.cpp \
#    vapour_flux_air.cpp \
#    vapour_flux_base.cpp \
#    vapour_flux_condensation.cpp \
#    vapour_flux_sum.cpp \
#    vapour_flux_sum_base.cpp \
#    vapour_flux_transpiration.cpp \
#    ventilated_latent_heat_converter.cpp \
#    ventilation_by_temp.cpp \
#    ventilation_by_wind.cpp \
#    vent.cpp \
#    vents.cpp

