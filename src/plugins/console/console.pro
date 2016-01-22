#-------------------------------------------------
#
# console DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = console

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

# What we are building
TEMPLATE = lib
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
TARGET = console$${SUFFIX}

# Location of library when installed on Unix
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Own library source code on which we depend
DEPENDPATH += $$_PRO_FILE_PWD_/../../plugins/base

# Additional folders with header files
INCLUDEPATH += "$$_PRO_FILE_PWD_/../../plugins"

HEADERS +=  \
    environment.h \
    history.h
	
SOURCES +=  \
    environment.cpp \
    history.cpp

