#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = aphid

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    crop_grows.h\
    wheat_growth.h\
    aphid_migration.h\
    aphid_migration2.h\
    potential_fecundity_Duffy.h\
    R0_Schmitz.h\
    life_fec_infected.h\
    nb_tillersD.h\
    density_per_tiller.h \
    Fecundity.h\
    Fecundity_Schmitz.h\
    Fecundity_Schmitz2.h\
    aph_intrinsic_mortality.h\
    aph_intrinsic_mortalityD.h\
    aph_survivalD.h\
    aph_survivalP.h\
    Infection_rate.h\
    Infection_rate2.h\
    cadavers_units.h\
    cadavers_units2.h\
    humidity_switch.h\
    Spo_cadavers.h\
    contamination.h\
            wheat_start2.h \
            alate3.h \
            aph_fecundity.h \
            aphid_start_simple.h \
            fungalMortality.h \
            fungus_start_simple.h \
            growthRate.h \
            infection_pressure.h

SOURCES +=  \
    crop_grows.cpp\
    wheat_growth.cpp\
    aphid_migration.cpp\
    aphid_migration2.cpp\
    potential_fecundity_Duffy.cpp\
    R0_Schmitz.cpp\
    life_fec_infected.cpp\
    nb_tillersD.cpp\
    density_per_tiller.cpp \
    Fecundity.cpp\
    Fecundity_Schmitz.cpp\
    Fecundity_Schmitz2.cpp\
    aph_intrinsic_mortality.cpp\
    aph_intrinsic_mortalityD.cpp\
    aph_survivalD.cpp\
    aph_survivalP.cpp\
    Infection_rate.cpp\
    Infection_rate2.cpp\
    cadavers_units.cpp\
    cadavers_units2.cpp\
    humidity_switch.cpp\
    Spo_cadavers.cpp\
    contamination.cpp\
            wheat_start2.cpp \
            alate3.cpp \
            aph_fecundity.cpp \
            aphid_start_simple.cpp \
            fungalMortality.cpp \
            fungus_start_simple.cpp \
            growthRate.cpp \
            infection_pressure.cpp

