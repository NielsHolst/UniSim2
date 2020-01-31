#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = vg

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    accumulator.h \
    actuators.h \
    air_flux_base.h \
    air_flux_given.h \
    air_flux_infiltration.h \
    air_flux_vents.h \
    any_flag.h \
    average.h \
    boundary_layer_resistance_base.h \
    boundary_layer_resistance_jones_a.h \
    boundary_layer_resistance_jones_b.h \
    boundary_layer_resistance_stanghellini.h \
    boundary_layer_resistance_monteith_unsworth.h \
    budget.h \
    chalk.h \
    controlled.h \
    controllers.h \
    controllers_input.h \
    cover.h \
    crop.h \
    crop_development.h \
    crop_mass.h \
    crop_growth.h \
    crop_lai.h \
    crop_radiation.h \
    crop_yield.h \
    daylight_level.h \
    diffuse_irradiation_base.h \
    diffuse_irradiation_RE.h \
    energy_flux_air.h \
    energy_flux_base.h \
    energy_flux_condensation.h \
    energy_flux_floor.h \
    energy_flux_sunlight.h \
    energy_flux_heating.h \
    energy_flux_shelter.h \
    energy_flux_sum.h \
    energy_flux_transpiration.h \
    energy_screen_balance_signal.h \
    floor_radiation_absorbed.h \
    fruit_crop_lai.h \
    fruit_crop_mass.h \
    fruit_factor.h \
    geometry.h \
    given.h \
    growth_light_base.h \
    growth_light_controller.h \
    growth_light.h \
    growth_lights.h \
    hump.h \
    ig_indoors_humidity.h \
    indoors.h \
    indoors_co2.h \
    indoors_humidity.h \
    indoors_light.h \
    indoors_temperature.h \
    indoors_wind_speed.h \
    leaf_layer.h \
    leaf_light_response.h \
    leaf_light_response_processes.h \
    leaf_photosynthesis.h \
    leaf_radiation_absorbed.h \
    leaf_temperature.h \
    leaf_transpiration.h \
    leaf_wind_speed.h \
    maximum.h \
    minimum.h \
    outdoors.h \
    pid_controller.h \
    pipe.h \
    pipe_forced.h \
    running_average.h \
    screen.h \
    screen_combination.h \
    screens.h \
    sensor.h \
    setpoints.h \
    setpoints_input.h \
    shelter.h \
    shelter_base.h \
    shelter_face.h \
    simulation_period.h \
    sky_temperature.h \
    stomatal_resistance_base.h \
    stomatal_resistance_rose.h \
    stomatal_resistance_tomato.h \
    sum.h \
    surface_radiation.h \
    surface_radiation_outputs.h \
    utilities.h \
    vapour_flux_air.h \
    vapour_flux_base.h \
    vapour_flux_condensation.h \
    vapour_flux_sum.h \
    vapour_flux_sum_base.h \
    vapour_flux_transpiration.h \
    vent.h \
    ventilated_latent_heat_converter.h

SOURCES += \
    accumulator.cpp \
    actuators.cpp \
    air_flux_base.cpp \
    air_flux_given.cpp \
    air_flux_infiltration.cpp \
    air_flux_vents.cpp \
    any_flag.cpp \
    average.cpp \
    boundary_layer_resistance_base.cpp \
    boundary_layer_resistance_jones_a.cpp \
    boundary_layer_resistance_jones_b.cpp \
    boundary_layer_resistance_stanghellini.cpp \
    boundary_layer_resistance_monteith_unsworth.cpp \
    budget.cpp \
    chalk.cpp \
    controlled.cpp \
    controllers.cpp \
    controllers_input.cpp \
    cover.cpp \
    crop.cpp \
    crop_development.cpp \
    crop_mass.cpp \
    crop_growth.cpp \
    crop_lai.cpp \
    crop_radiation.cpp \
    crop_yield.cpp \
    daylight_level.cpp \
    diffuse_irradiation_base.cpp \
    diffuse_irradiation_RE.cpp \
    energy_flux_air.cpp \
    energy_flux_base.cpp \
    energy_flux_condensation.cpp \
    energy_flux_floor.cpp \
    energy_flux_sunlight.cpp \
    energy_flux_heating.cpp \
    energy_flux_shelter.cpp \
    energy_flux_sum.cpp \
    energy_flux_transpiration.cpp \
    energy_screen_balance_signal.cpp \
    floor_radiation_absorbed.cpp \
    fruit_crop_lai.cpp \
    fruit_crop_mass.cpp \
    fruit_factor.cpp \
    geometry.cpp \
    given.cpp \
    growth_light_base.cpp \
    growth_light_controller.cpp \
    growth_light.cpp \
    growth_lights.cpp \
    hump.cpp \
    ig_indoors_humidity.cpp \
    indoors.cpp \
    indoors_co2.cpp \
    indoors_humidity.cpp \
    indoors_light.cpp \
    indoors_temperature.cpp \
    indoors_wind_speed.cpp \
    leaf_layer.cpp \
    leaf_light_response.cpp \
    leaf_light_response_processes.cpp \
    leaf_photosynthesis.cpp \
    leaf_radiation_absorbed.cpp \
    leaf_temperature.cpp \
    leaf_transpiration.cpp \
    leaf_wind_speed.cpp \
    maximum.cpp \
    minimum.cpp \
    outdoors.cpp \
    pid_controller.cpp \
    pipe.cpp \
    pipe_forced.cpp \
    running_average.cpp \
    screen.cpp \
    screen_combination.cpp \
    screens.cpp \
    sensor.cpp \
    setpoints.cpp \
    setpoints_input.cpp \
    shelter.cpp \
    shelter_base.cpp \
    shelter_face.cpp \
    simulation_period.cpp \
    sky_temperature.cpp \
    stomatal_resistance_base.cpp \
    stomatal_resistance_rose.cpp \
    stomatal_resistance_tomato.cpp \
    sum.cpp \
    surface_radiation.cpp \
    surface_radiation_outputs.cpp \
    utilities.cpp \
    vapour_flux_air.cpp \
    vapour_flux_base.cpp \
    vapour_flux_condensation.cpp \
    vapour_flux_sum.cpp \
    vapour_flux_sum_base.cpp \
    vapour_flux_transpiration.cpp \
    vent.cpp \
    ventilated_latent_heat_converter.cpp
