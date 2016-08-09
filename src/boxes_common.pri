#-------------------------------------------------
#
# Project to include in all boxes projects
# 
# Usage
# include("$$_PRO_FILE_PWD_/../../boxes_common.pri") 
#-------------------------------------------------

# Build one or the other version
CONFIG += debug
#CONFIG += release

# Set suffix 'd' for debug version
CONFIG(debug, debug|release) {
    SUFFIX = _d
}
else {
    SUFFIX = _r
}

# What we are building
CONFIG += console c++11
CONFIG -= app_bundle # do not bundle app on Mac OS
QT += core 
QT -= gui

# Compiler options to silence warnings when compiling Boost
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs -Wattributes

# Compiler options to speed up code
QMAKE_CXXFLAGS += -ffast-math

# Additional folders with header files
BOOST_PATH = $$(BOOST_ROOT)
!isEmpty(BOOST_PATH) {
    INCLUDEPATH += "$$(BOOST_ROOT)"
}

# Own libraries that we use, except 'base' does not itself use 'base'
!equals(BOXES_PLUGIN_NAME, "base") {
    win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbase_r
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbase_d
    else:unix:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbase_r
    else:unix:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbase_d
}
