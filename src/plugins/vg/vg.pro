#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = vg

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    actuators.h \
    actuator_heat_pipes.h \
    actuator_ventilation.h \
    any_flag.h \
    boundary_layer_resistance_base.h \
    boundary_layer_resistance_jones_a.h \
    boundary_layer_resistance_jones_b.h \
    boundary_layer_resistance_stanghellini.h \
    boundary_layer_resistance_monteith_unsworth.h \
    budget.h \
    chalk.h \
    controllers.h \
    controller_ventilation_combined.h \
    cover.h \
    crop.h \
    crop_growth.h \
    diffuse_irradiation_base.h \
    diffuse_irradiation_RE.h \
    energy_budget.h \
    energy_budget_optimiser.h \
    geometry.h \
    growth_light_controller.h \
    growth_light.h \
    growth_lights.h \
    heat_transfer_layer_base.h \
    heat_transfer_layer_parameters.h \
    heat_transfer_cover.h \
    heat_transfer_crop.h \
    heat_transfer_floor.h \
    heat_transfer_pipe.h \
    heat_transfer_screen1.h \
    heat_transfer_screen2.h \
    heat_transfer_screen3.h \
    heat_transfer_shelter.h \
    heat_transfer_sky.h \
    ig_indoors_humidity.h \
    indoors.h \
    indoors_co2.h \
    indoors_humidity.h \
    indoors_light.h \
    indoors_temperature.h \
    indoors_ventilation.h \
    indoors_wind_speed.h \
    leaf_light_response.h \
    leaf_light_response_processes.h \
    leaf_photosynthesis.h \
    leaf_temperature.h \
    leaf_transpiration.h \
    leaf_wind_speed.h \
    leakage_ventilation.h \
    outdoors.h \
    outputs.h \
    pid_controller.h \
    pipe.h \
    pipe_forced.h \
    screen.h \
    screen_roof.h \
    screen_wall.h \
    screen_combination.h \
    screens.h \
    screens_air_transmissivity.h \
    sensor.h \
    setpoints.h \
    setpoint_heating.h \
    setpoint_ventilation.h \
    shelter.h \
    shelter_face.h \
    shelter_face_area.h \
    simulation_period.h \
    sky_temperature.h \
    stomatal_resistance_base.h \
    stomatal_resistance_rose.h \
    stomatal_resistance_tomato.h \
    utilities.h \
    vapour_flux_base.h \
    vapour_flux_condensation_cover.h \
    vapour_flux_condensation_crop.h \
    vapour_flux_condensation_screen_base.h \
    vapour_flux_condensation_screen1.h \
    vapour_flux_condensation_screen2.h \
    vapour_flux_condensation_screen3.h \
    vapour_flux_condensation_screens.h \
    vapour_flux_transpiration.h \
    vapour_flux_ventilation.h \
    vent.h \
    ventilated_latent_heat_converter.h \
    vg_documentation.h \
    water_budget.h

SOURCES += \
    actuators.cpp \
    actuator_heat_pipes.cpp \
    actuator_ventilation.cpp \
    any_flag.cpp \
    boundary_layer_resistance_base.cpp \
    boundary_layer_resistance_jones_a.cpp \
    boundary_layer_resistance_jones_b.cpp \
    boundary_layer_resistance_stanghellini.cpp \
    boundary_layer_resistance_monteith_unsworth.cpp \
    budget.cpp \
    chalk.cpp \
    controllers.cpp \
    controller_ventilation_combined.cpp \
    cover.cpp \
    crop.cpp \
    crop_growth.cpp \
    diffuse_irradiation_base.cpp \
    diffuse_irradiation_RE.cpp \
    energy_budget.cpp \
    energy_budget_optimiser.cpp \
    geometry.cpp \
    growth_light_controller.cpp \
    growth_light.cpp \
    growth_lights.cpp \
    heat_transfer_layer_base.cpp \
    heat_transfer_layer_parameters.cpp \
    heat_transfer_cover.cpp \
    heat_transfer_crop.cpp \
    heat_transfer_floor.cpp \
    heat_transfer_pipe.cpp \
    heat_transfer_screen1.cpp \
    heat_transfer_screen2.cpp \
    heat_transfer_screen3.cpp \
    heat_transfer_shelter.cpp \
    heat_transfer_sky.cpp \
    ig_indoors_humidity.cpp \
    indoors.cpp \
    indoors_co2.cpp \
    indoors_humidity.cpp \
    indoors_light.cpp \
    indoors_temperature.cpp \
    indoors_ventilation.cpp \
    indoors_wind_speed.cpp \
    leaf_light_response.cpp \
    leaf_light_response_processes.cpp \
    leaf_photosynthesis.cpp \
    leaf_temperature.cpp \
    leaf_transpiration.cpp \
    leaf_wind_speed.cpp \
    leakage_ventilation.cpp \
    outdoors.cpp \
    outputs.cpp \
    pid_controller.cpp \
    pipe.cpp \
    pipe_forced.cpp \
    screen.cpp \
    screen_roof.cpp \
    screen_wall.cpp \
    screen_combination.cpp \
    screens.cpp \
    screens_air_transmissivity.cpp \
    sensor.cpp \
    setpoints.cpp \
    setpoint_heating.cpp \
    setpoint_ventilation.cpp \
    shelter.cpp \
    shelter_face.cpp \
    shelter_face_area.cpp \
    simulation_period.cpp \
    sky_temperature.cpp \
    stomatal_resistance_base.cpp \
    stomatal_resistance_rose.cpp \
    stomatal_resistance_tomato.cpp \
    utilities.cpp \
    vapour_flux_base.cpp \
    vapour_flux_condensation_cover.cpp \
    vapour_flux_condensation_crop.cpp \
    vapour_flux_condensation_screen_base.cpp \
    vapour_flux_condensation_screen1.cpp \
    vapour_flux_condensation_screen2.cpp \
    vapour_flux_condensation_screen3.cpp \
    vapour_flux_condensation_screens.cpp \
    vapour_flux_transpiration.cpp \
    vapour_flux_ventilation.cpp \
    vent.cpp \
    ventilated_latent_heat_converter.cpp \
    vg_documentation.cpp \
    water_budget.cpp
