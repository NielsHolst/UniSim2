#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    eab_fecundity.h \
    fibonacci.h \
    jump.h \
    egg_development.h
	
SOURCES +=  \
    eab_fecundity.cpp \
    fibonacci.cpp \
    jump.cpp \
    egg_development.cpp


