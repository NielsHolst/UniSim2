#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
#    mediator.h \
    big_bang.h \
    pollinator.h \
    plant.h \
    fibonacci.h \
    jump.h
	
SOURCES +=  \
#    mediator.cpp \
    big_bang.cpp \
    pollinator.cpp \
    plant.cpp \
    fibonacci.cpp \
    jump.cpp

