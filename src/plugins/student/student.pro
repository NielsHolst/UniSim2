#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    eab_fecundity.h \
    egg_development.h \
    fibonacci.h \
    jump.h
	
SOURCES +=  \
    eab_fecundity.cpp \
    egg_development.cpp \
    fibonacci.cpp \
    jump.cpp

