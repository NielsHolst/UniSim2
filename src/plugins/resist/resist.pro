#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = resist

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    allele_frequency.h \
    carrying_capacity.h \
    check_vector.h \
    density.h \
    genotype_frequency.h \
    immigration.h \
    population.h \
    proportion_bounds.h \
    reproduction.h \
    reproductive_rate.h \
    spray_response.h \
    survival.h
	
SOURCES +=  \
    allele_frequency.cpp \
    carrying_capacity.cpp \
    density.cpp \
    genotype_frequency.cpp \
    immigration.cpp \
    population.cpp \
    proportion_bounds.cpp \
    reproduction.cpp \
    reproductive_rate.cpp \
    spray_response.cpp \
    survival.cpp


