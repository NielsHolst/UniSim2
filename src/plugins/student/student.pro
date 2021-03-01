#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    coffee_summary.h \
    fibonacci.h \
    jump.h \
    rain_mortality.h \
    student_documentation.h
SOURCES +=  \
    coffee_summary.cpp \
    fibonacci.cpp \
    jump.cpp \
    rain_mortality.cpp \
    student_documentation.cpp


