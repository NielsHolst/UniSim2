#-------------------------------------------------
#
# boxes DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = boxes

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    b.h \
    briere.h \
    buffer.h \
    calendar.h \
    date.h \
    day_degrees.h \
    date_time_signal.h \
    demand_budget.h \
    distributed_delay_base.h \
    distributed_delay.h \
    distributed_delay_2d.h \
    food_web.h \
    food_web_box.h \
    functional_response.h \
    layout_r.h \
    maker.h \
    numbered_file.h \
    offset_date_time.h \
    on_off.h \
    output_buffer.h \
    output_ports.h \
    output_r.h \
    output_text.h \
    page_r.h \
    plot_r.h \
    population.h \
    priority_signal.h \
    proportional_signal.h \
    randomiser_base.h \
    randomiser_monte_carlo.h \
    randomiser_sobol_sequence.h \
    randomiser_stratified.h \
    random_base.h \
    random_base_typed.h \
    random_binomial.h \
    random_lognormal.h \
    random_normal.h \
    random_uniform.h \
    random_uniform_int.h \
    ratio.h \
    records.h \
    sandy.h \
    scenarios.h \
    select_file.h \
    sequence.h \
    simulation.h \
    sine_wave_temperature.h \
    sliding_signal.h \
    stage_base.h \
    stage.h \
    stage_and_phase.h \
    super_functional_response.h \
    supply_budget.h \
    threshold_signal.h \
    vector_scaled.h \
    vector_sum.h
	
SOURCES +=  \
    b.cpp \
    briere.cpp \
    buffer.cpp \
    calendar.cpp \
    date.cpp \
    date_time_signal.cpp \
    day_degrees.cpp \
    demand_budget.cpp \
    distributed_delay_base.cpp \
    distributed_delay.cpp \
    distributed_delay_2d.cpp \
    food_web.cpp \
    food_web_box.cpp \
    functional_response.cpp \
    layout_r.cpp \
    maker.cpp \
    numbered_file.cpp \
    offset_date_time.cpp \
    on_off.cpp \
    output_buffer.cpp \
    output_ports.cpp \
    output_r.cpp \
    output_text.cpp \
    page_r.cpp \
    plot_r.cpp \
    population.cpp \
    priority_signal.cpp \
    proportional_signal.cpp \
    randomiser_base.cpp \
    randomiser_monte_carlo.cpp \
    randomiser_sobol_sequence.cpp \
    randomiser_stratified.cpp \
    random_base.cpp \
    random_binomial.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_uniform.cpp \
    random_uniform_int.cpp \
    ratio.cpp \
    records.cpp \
    sandy.cpp \
    scenarios.cpp \
    select_file.cpp \
    sequence.cpp \
    simulation.cpp \
    sine_wave_temperature.cpp \
    sliding_signal.cpp \
    stage_base.cpp \
    stage.cpp \
    stage_and_phase.cpp \
    super_functional_response.cpp \
    supply_budget.cpp \
    threshold_signal.cpp \
    vector_scaled.cpp \
    vector_sum.cpp


