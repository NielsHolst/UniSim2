#-------------------------------------------------
#
# boxes test app
#
#-------------------------------------------------
BOXES_APP_NAME = test

include("$$_PRO_FILE_PWD_/../../boxes_app.pri")

# Qt libraries that we use
QT += testlib

# Our project source code
HEADERS +=  \
    autotest.h \
    test_assign.h \
    test_box.h \
    test_box_builder.h \
    test_box_cases.h \
    test_command.h \
    test_general.h \
    test_history.h \
    test_path.h \
    test_port_type.h \
    test_port.h \
    test_savanna.h \
    test_vectorization.h

SOURCES +=  \
    main.cpp \
    test_assign.cpp \
    test_box.cpp \
    test_box_builder.cpp \
    test_box_cases.cpp \
    test_command.cpp \
    test_general.cpp \
    test_history.cpp \
    test_path.cpp \
    test_port_type.cpp \
    test_port.cpp \
    test_savanna.cpp \
    test_vectorization.cpp

# Load console library
win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsoled
else:unix:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsole
else:unix:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsoled
