#-------------------------------------------------
#
#  app
#
#-------------------------------------------------
BOXES_APP_NAME = igclient

include("$$_PRO_FILE_PWD_/../../boxes_app.pri")

# Replace file resource
RESOURCES = "$$_PRO_FILE_PWD_/../../resources/igclient.qrc"

# Qt libraries that we use
QT       -= gui
CONFIG   += console
LIBS += -L$$_PRO_FILE_PWD_/../../../bin/lib -liglib$${SUFFIX}

# Where to look for iglib headers
INCLUDEPATH += $$_PRO_FILE_PWD_/../../lib

# Our project source code
SOURCES +=  \
    main.cpp

