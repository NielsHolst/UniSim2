#-------------------------------------------------
#
# unisimlib_test_client app
#
#-------------------------------------------------
BOXES_APP_NAME = unisimlib_test_client

include("$$_PRO_FILE_PWD_/../../boxes_app.pri")

# Qt libraries that we use
QT       -= gui
CONFIG   += console
LIBS += -L$$_PRO_FILE_PWD_/../../../bin/lib -lunisimlib_test$${SUFFIX}

# Our project source code
HEADERS +=  \
    unisimlib_test.h
    
SOURCES +=  \
    main.cpp

