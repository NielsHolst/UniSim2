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
    random_base.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    sandy.cpp \
    sequence.cpp \
    simulation.cpp


