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
    output_file.h \
    test_any_year.h \
    test_assign.h \
    test_ast_boxes.h \
    test_box.h \
    test_box_builder.h \
    test_box_cases.h \
    test_box_script.h \
    test_calendar.h \
    test_circle_buffer.h \
    test_command.h \
    test_convert.h \
    test_copy_folder.h \
    test_data_frame.h \
    test_distribution.h \
    test_eq.h \
    test_general_split.h \
    test_history.h \
    test_maker.h \
    test_matrix.h \
    test_new_port.h \
    test_output_buffer.h \
    test_output_text.h \
    test_path.h \
#    test_population.h \
    test_port_buffer.h \
    test_port_type.h \
    test_port.h \
    test_random.h \
    test_savanna.h \
    test_scenarios.h \
    test_simulation.h \
    test_track.h \
    test_unique_name.h \
    test_vector.h \
    test_vector_op.h \
    test_vector_sum.h \
    test_vectorization.h \
    test_weather.h \
    input_file_path.h

SOURCES +=  \
    main.cpp \
    output_file.cpp \
    test_any_year.cpp \
    test_assign.cpp \
    test_ast_boxes.cpp \
    test_box.cpp \
    test_box_builder.cpp \
    test_box_cases.cpp \
    test_box_script.cpp \
    test_calendar.cpp \
    test_circle_buffer.cpp \
    test_command.cpp \
    test_convert.cpp \
    test_copy_folder.cpp \
    test_data_frame.cpp \
    test_distribution.cpp \
    test_eq.cpp \
    test_general_split.cpp \
    test_history.cpp \
    test_maker.cpp \
    test_matrix.cpp \
    test_new_port.cpp \
    test_output_buffer.cpp \
    test_output_text.cpp \
    test_path.cpp \
#    test_population.cpp \
    test_port_buffer.cpp \
    test_port_type.cpp \
    test_port.cpp \
    test_random.cpp \
    test_savanna.cpp \
    test_scenarios.cpp \
    test_simulation.cpp \
    test_track.cpp \
    test_unique_name.cpp \
    test_vector.cpp \
    test_vector_op.cpp \
    test_vector_sum.cpp \
    test_vectorization.cpp \
    test_weather.cpp
