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
VERSION = 2.3.16
CONFIG += skip_target_version_ext
# AUTO-CONFIG-END

# Set suffix 'd' for debug version
CONFIG(debug, debug|release) {
    SUFFIX = d
}
else {
    SUFFIX =
}

# What we are building
CONFIG += c++17
QT += core widgets

# Turn warnings off in debug mode
#CONFIG(debug, debug|release) {
#  CONFIG += warn_off
#}

# Show warning when using deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS

# Compiler options add-ons
QMAKE_CXXFLAGS += -Wall -Wextra

# Compiler options to silence warnings
QMAKE_CXXFLAGS += -Wno-padded

# Compiler options to silence warnings when compiling Boost
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs -Wno-attributes -Wno-deprecated-declarations
DEFINES += BOOST_ALLOW_DEPRECATED_HEADERS

# Compiler options to silence warnings when compiling under Mac OS X
MY_HASH = $$LITERAL_HASH
NO_PRAGMA_MESSAGES = $$join(MY_HASH, , -Wno-\\, pragma-messages)
macx:QMAKE_CXXFLAGS += $${NO_PRAGMA_MESSAGES} -Wno-inconsistent-missing-override

# Compiler options to silence warnings when compiling under Win and Linux
!macx:QMAKE_CXXFLAGS += -Wno-unknown-pragmas

# Compiler options to speed up code
QMAKE_CXXFLAGS += -ffast-math -O3

# Compiler option that would have saved me some days' work
QMAKE_CXXFLAGS += -Wdelete-non-virtual-dtor

# Compiler flags for gcc / gdb version incompatibility
# https://stackoverflow.com/questions/16611678/how-to-make-the-locals-and-expressions-debugging-window-operational-with-gcc-4
## QMAKE_CXXFLAGS += -gdwarf-3

# Additional folders with header files
BOOST_PATH = $$(BOOST_ROOT)
isEmpty(BOOST_PATH) {
    BOOST_PATH = $$_PRO_FILE_PWD_/../../../../boost
}
INCLUDEPATH += $${BOOST_PATH}

# Own libraries that we use, except 'base' does not use itself
!equals(BOXES_PLUGIN_NAME, "base") {
    win32:LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -luniversal_simulator_base$${SUFFIX}
    unix:LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -luniversal_simulator_base$${SUFFIX}
    macx:LIBS += -L$$(HOME)/lib -luniversal_simulator_base$${SUFFIX}
}
