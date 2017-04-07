#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    mediator.h \
    pollinator.h \
    plant.h \
    fibonacci.h \
    jump.h
	
SOURCES +=  \
    mediator.cpp \
    pollinator.cpp \
    plant.cpp \
    fibonacci.cpp \
    jump.cpp

