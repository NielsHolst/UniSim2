#-------------------------------------------------
#
# Project to include in all app projects
# 
# Usage:
# BOXES_APP_NAME = test
# include("$$_PRO_FILE_PWD_/../../app.pri")
# HEADERS += ...
# SOURCES += ...
# 
#-------------------------------------------------

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

# What we are building
TEMPLATE = app
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
TARGET = $$BOXES_APP_NAME$${SUFFIX}

# Own library source code on which we depend
DEPENDPATH += $$_PRO_FILE_PWD_/../../plugins/boxes

# Additional folders with header files
INCLUDEPATH += "$$_PRO_FILE_PWD_/../../plugins"
