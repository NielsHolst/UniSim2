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
    exception_expectation.h \
    input_file_path.h \
    test_boost_variant.h \
    test_box_builder.h \
    test_box_cases.h \
    test_box_script_x3.h \
    test_convert_date.h \
    test_convert_date_time.h \
    test_convert_time.h \
    test_convert_vector.h \
    test_expression.h \
    test_operate.h \
    test_spirit.h \
    test_value.h \

SOURCES +=  \
    main.cpp \
    test_boost_variant.cpp \
    test_box_builder.cpp \
    test_box_cases.cpp \
    test_box_script_x3.cpp \
    test_convert_date.cpp \
    test_convert_date_time.cpp \
    test_convert_time.cpp \
    test_convert_vector.cpp \
    test_expression.cpp \
    test_operate.cpp \
    test_spirit.cpp \
    test_value.cpp \
