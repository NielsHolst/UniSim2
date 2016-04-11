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
    test_circle_buffer.h \
    test_command.h \
    test_convert.h \
    test_general_split.h \
    test_history.h \
    test_path.h \
    test_port_buffer.h \
    test_port_type.h \
    test_port.h \
    test_savanna.h \
    test_simulation.h \
    test_track.h \
    test_unique_name.h \
    test_vector.h \
    test_vectorization.h \
    test_weather.h

SOURCES +=  \
    main.cpp \
    test_assign.cpp \
    test_box.cpp \
    test_box_builder.cpp \
    test_box_cases.cpp \
    test_circle_buffer.cpp \
    test_command.cpp \
    test_convert.cpp \
    test_general_split.cpp \
    test_history.cpp \
    test_path.cpp \
    test_port_buffer.cpp \
    test_port_type.cpp \
    test_port.cpp \
    test_savanna.cpp \
    test_simulation.cpp \
    test_track.cpp \
    test_unique_name.cpp \
    test_vector.cpp \
    test_vectorization.cpp \
    test_weather.cpp
