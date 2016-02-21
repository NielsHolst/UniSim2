#-------------------------------------------------
#
# boxes DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = boxes

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    calendar.h \
    fibonacci.h \
    output_r.h \
    page_r.h \
    plot_r.h \
    random_base.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    sandy.h \
    sequence.h \
    simulation.h
	
SOURCES +=  \
    calendar.cpp \
    fibonacci.cpp \
    output_r.cpp \
    page_r.cpp \
    plot_r.cpp \
    random_base.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    sandy.cpp \
    sequence.cpp \
    simulation.cpp


