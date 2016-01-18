#-------------------------------------------------
#
# base DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = base

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    random_base.h \
    random_lognormal.h \
    random_normal.h \
    random_poisson.h \
    random_uniform.h \
    simulation.h
	
SOURCES +=  \
    random_base.cpp \
    random_lognormal.cpp \
    random_normal.cpp \
    random_poisson.cpp \
    random_uniform.cpp \
    simulation.cpp


