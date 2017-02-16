#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = aphid

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    alate3.h \
    aph_fecundity.h \
		aphid_start_simple.h \
		density_per_tiller.h \
		fungalMortality.h \
		fungus_start_simple.h \
		growthRate.h \
		infection_pressure.h \
		wheat_start2.h
	
SOURCES +=  \
    alate3.cpp \
    aph_fecundity.cpp \
		aphid_start_simple.cpp \
		density_per_tiller.cpp \
		fungalMortality.cpp \
		fungus_start_simple.cpp \
		growthRate.cpp \
		infection_pressure.cpp \
		wheat_start2.cpp

