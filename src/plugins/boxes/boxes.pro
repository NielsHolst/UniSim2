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
    fibonacci.h \
    layout_r.h \
    output_r.h \
    page_r.h \
    plot_r.h \
    population.h \
    random_base.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    records.h \
    sandy.h \
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
    fibonacci.cpp \
    layout_r.cpp \
    output_r.cpp \
    page_r.cpp \
    plot_r.cpp \
    population.cpp \
    random_base.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    records.cpp \
    sandy.cpp \
    sequence.cpp \
    simulation.cpp \
    stage_base.cpp \
    stage.cpp \
    vector_sum.cpp \
    weather.cpp


