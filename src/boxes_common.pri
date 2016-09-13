#-------------------------------------------------
#
# Project to include in all boxes projects
# 
# Usage
# include("$$_PRO_FILE_PWD_/../../boxes_common.pri") 
#-------------------------------------------------

# Build one or the other version
# AUTO-CONFIG-BEGIN
CONFIG += release
# AUTO-CONFIG-END

# Set suffix 'd' for debug version
CONFIG(debug, debug|release) {
    SUFFIX = d
}
else {
    SUFFIX =
}

# What we are building
CONFIG += c++11
QT += core 

# Compiler options to silence warnings when compiling Boost
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs -Wattributes

# Compiler options to speed up code
QMAKE_CXXFLAGS += -ffast-math

# Additional folders with header files
BOOST_PATH = $$(BOOST_ROOT)
isEmpty(BOOST_PATH) {
    error("Cannot find enviroment variable BOOST_ROOT")
}
INCLUDEPATH += "$$(BOOST_ROOT)"

# Own libraries that we use, except 'base' does not itself use 'base'
!equals(BOXES_PLUGIN_NAME, "base") {
    win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbase
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbased
    else:unix:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbase
    else:unix:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lbased
}
