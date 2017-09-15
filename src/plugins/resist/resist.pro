#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = resist

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    carrying_capacity.h \
    check_vector.h \
    density.h \
    genotype_frequency.h \
    immigration.h \
    population.h \
    reproduction.h \
    reproductive_rate.h \
    spray_response.h \
    survival.h
	
SOURCES +=  \
    carrying_capacity.cpp \
    density.cpp \
    genotype_frequency.cpp \
    immigration.cpp \
    population.cpp \
    reproduction.cpp \
    reproductive_rate.cpp \
    spray_response.cpp \
    survival.cpp


