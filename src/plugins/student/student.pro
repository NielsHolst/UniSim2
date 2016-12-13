#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    alate2.h \
    aph_fecundity.h \
    density_per_tiller.h \
    eab_fecundity.h \
    egg_development.h \
    fibonacci.h \
    jump.h
	
SOURCES +=  \
    alate2.cpp \
    aph_fecundity.cpp \
    density_per_tiller.cpp \
    eab_fecundity.cpp \
    egg_development.cpp \
    fibonacci.cpp \
    jump.cpp

