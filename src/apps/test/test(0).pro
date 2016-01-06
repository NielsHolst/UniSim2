#-------------------------------------------------
#
# boxes test app
#
#-------------------------------------------------
message("test")
message($$_PRO_FILE_PWD_)

BOXES_APP_NAME = test

include("$$_PRO_FILE_PWD_/../../boxes_app.pri")

# Qt libraries that we use
QT += testlib

# Our project source code
HEADERS +=  \
    autotest.h \
    test_box.h \
    test_box_builder.h \
    test_box_cases.h \
    test_path.h \
    test_port.h \
    test_savanna.h

SOURCES +=  \
    main.cpp \
    test_box.cpp \
    test_box_builder.cpp \
    test_box_cases.cpp \
    test_path.cpp \
    test_port.cpp \
    test_savanna.cpp
