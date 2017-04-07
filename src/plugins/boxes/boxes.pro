#-------------------------------------------------
#
# boxes DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = boxes

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    buffer.h \
    calendar.h \
    day_degrees.h \
    distributed_delay_base.h \
    distributed_delay.h \
    layout_r.h \
    maker.h \
    output_buffer.h \
    output_ports.h \
    output_r.h \
    output_text.h \
    page_r.h \
    plot_r.h \
    population.h \
    random_base.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    random_uniform_int.h \
    records.h \
    sandy.h \
    scenarios.h \
    sensitivity_analysis.h \
    sequence.h \
    simulation.h \
    stage_base.h \
    stage.h \
    vector_sum.h \
    weather.h
	
SOURCES +=  \
    buffer.cpp \
    calendar.cpp \
    day_degrees.cpp \
    distributed_delay_base.cpp \
    distributed_delay.cpp \
    layout_r.cpp \
    maker.cpp \
    output_buffer.cpp \
    output_ports.cpp \
    output_r.cpp \
    output_text.cpp \
    page_r.cpp \
    plot_r.cpp \
    population.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    random_uniform_int.cpp \
    records.cpp \
    sandy.cpp \
    scenarios.cpp \
    sensitivity_analysis.cpp \
    sequence.cpp \
    simulation.cpp \
    stage_base.cpp \
    stage.cpp \
    vector_sum.cpp \
    weather.cpp


