#-------------------------------------------------
#
# boxes DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = boxes

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    briere.h \
    buffer.h \
    calendar.h \
    day_degrees.h \
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
    on_off.h \
    output_buffer.h \
    output_ports.h \
    output_r.h \
    output_text.h \
    page_r.h \
    plot_r.h \
    population.h \
    random_base.h \
    random_binomial.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    random_uniform_int.h \
    records.h \
    sandy.h \
    scenarios.h \
    sensitivity_analysis_base.h \
    sensitivity_analysis_simple.h \
    sensitivity_analysis_sobol.h \
    sequence.h \
    simulation.h \
    stage_base.h \
    stage.h \
    stage_and_phase.h \
    supply_budget.h \
    vector_sum.h
	
SOURCES +=  \
    briere.cpp \
    buffer.cpp \
    calendar.cpp \
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
    on_off.cpp \
    output_buffer.cpp \
    output_ports.cpp \
    output_r.cpp \
    output_text.cpp \
    page_r.cpp \
    plot_r.cpp \
    population.cpp \
    random_binomial.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    random_uniform_int.cpp \
    records.cpp \
    sandy.cpp \
    scenarios.cpp \
    sensitivity_analysis_base.cpp \
    sensitivity_analysis_simple.cpp \
    sensitivity_analysis_sobol.cpp \
    sequence.cpp \
    simulation.cpp \
    stage_base.cpp \
    stage.cpp \
    stage_and_phase.cpp \
    supply_budget.cpp \
    vector_sum.cpp


