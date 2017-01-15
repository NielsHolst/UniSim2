#-------------------------------------------------
#
# Project to include in all boxes projects
# 
# Usage
# include("$$_PRO_FILE_PWD_/../../boxes_common.pri") 
#-------------------------------------------------

# Build one or the other version
# AUTO-CONFIG-BEGIN
CONFIG += debug
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
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs -Wno-attributes -Wno-deprecated-declarations

# Compiler options to silence warnings when compiling under Mac OS X
MY_HASH = $$LITERAL_HASH
NO_PRAGMA_MESSAGES = $$join(MY_HASH, , -Wno-\\, pragma-messages)
QMAKE_CXXFLAGS += $${NO_PRAGMA_MESSAGES} -Wno-inconsistent-missing-override

# Compiler options to speed up code
QMAKE_CXXFLAGS += -ffast-math

# Additional folders with header files
BOOST_PATH = $$(BOOST_ROOT)
isEmpty(BOOST_PATH) {
    error("Cannot find enviroment variable BOOST_ROOT")
}
INCLUDEPATH += "$$(BOOST_ROOT)"

# Own libraries that we use, except 'base' does not use itself
!equals(BOXES_PLUGIN_NAME, "base") {
    LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -luniversal_simulator_base$${SUFFIX}
}
