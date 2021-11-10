#-------------------------------------------------
#
# base DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = base

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

QT += widgets

# What we are building
TEMPLATE = lib
#CONFIG += lib_bundle
win32:DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
unix:DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
macx:DESTDIR = "~/lib"
TARGET = universal_simulator_base$${SUFFIX}

# Our project header and source code
HEADERS +=  \
    altova_xml.h \
    any_year.h \
    assign.h \
    ast_boxes.h \
    ast_common.h \
    bare_date.h \
    bare_date_time.h \
    base_signal.h \
    box.h \
    box_builder.h \
    box_output.h \
    box_preprocessor.h \
    box_reader_base.h \
    box_reader_boxes.h \
    box_reader_xml.h \
    boxscript.h \
    boxscript_ast.h \
    boxscript_ast_adapted.h \
    boxscript_config.h \
    boxscript_def.h \
    boxscript_error_handler.h \
    boxscript_parser.h \
    boxscript_skipper.h \
    caller.h \
    circular_buffer.h \
    command.h  \
    command_help.h  \
    computation_step.h \
    construction_step.h \
    convert.h \
    copy_folder.h \
    data_frame.h \
    data_grid.h \
    date_time.h \
    documentation.h \
    dialog.h \
    dialog_base.h \
    dialog_minimal.h \
    dialog_quiet.h \
    dialog_stub.h \
    dialog_widget.h \
    distribution.h \
    enum_functions.h \
    environment.h \
    exception.h \
    exception_context_class.h \
    expression_element.h \
    expression_operand.h \
    expression_operation.h \
    expression_operator.h \
    expression_stack.h \
    factory_plug_in.h \
    food_web_box_base.h \
    general.h \
    history.h \
    initialize.h \
    interpolate.h \
    matrix.h \
    matrix2D.h \
    mega_factory.h \
    minimise.h \
    object_pool.h \
    organisation.h \
    path.h \
    physiological_time.h \
    phys_math.h \
    port.h \
    port_access.h \
    port_buffer.h \
    port_filter.h \
    port_mode.h \
    port_transform.h \
#    port_type.h \
    port_value_op.h \
    product_base.h \
    product.h \
    proportions.h \
    psychrolib.h \
    random_generator.h \
    random_order.h \
    save_grammar_atom.h \
    save_grammar_base.h \
    save_grammar_notepad.h \
    save_graph_base.h \
    save_graph_graphviz.h \
    table.h \
    test_num.h \
    track.h \
    track_collection.h \
    time_with_units.h \
    timer.h \
    unique_name.h \
    vector.h \
    vector_op.h \
    vector_vector.h \
    vectorize.h \
    version.h \
    win_taskbar_progress.h

SOURCES +=  \
    altova_xml.cpp \
    any_year.cpp \
    assign.cpp \
    ast_boxes.cpp \
    ast_common.cpp \
    base_signal.cpp \
    box.cpp \
    box_builder.cpp \
    box_output.cpp \
    box_preprocessor.cpp \
    box_reader_base.cpp \
    box_reader_boxes.cpp \
    box_reader_xml.cpp \
    boxscript.cpp \
    boxscript_ast.cpp \
    boxscript_parser.cpp \
    caller.cpp \
    command.cpp \
    command_help.cpp  \
    computation_step.cpp \
    construction_step.cpp \
    convert.cpp \
    copy_folder.cpp \
    data_frame.cpp \
    data_grid.cpp \
    date_time.cpp \
    dialog.cpp \
    dialog_base.cpp \
    dialog_minimal.cpp \
    dialog_quiet.cpp \
    dialog_stub.cpp \
    dialog_widget.cpp \
    distribution.cpp \
    documentation.cpp \
    environment.cpp \
    exception.cpp \
    exception_context_class.cpp \
    expression_operand.cpp \
    expression_operation.cpp \
    expression_operator.cpp \
    expression_stack.cpp \
    food_web_box_base.cpp \
    general.cpp \
    history.cpp \
    initialize.cpp \
    mega_factory.cpp \
    object_pool.cpp \
    path.cpp \
    physiological_time.cpp \
    phys_math.cpp \
    port.cpp \
    port_access.cpp \
    port_buffer.cpp \
    port_filter.cpp \
    port_mode.cpp \
    port_transform.cpp \
#    port_type.cpp \
    port_value_op.cpp \
    product_base.cpp \
    proportions.cpp \
    psychrolib.c \
    random_generator.cpp \
    random_order.cpp \
    save_grammar_atom.cpp \
    save_grammar_base.cpp \
    save_grammar_notepad.cpp \
    save_graph_base.cpp \
    save_graph_graphviz.cpp \
    table.cpp \
    time_with_units.cpp \
    timer.cpp \
    track.cpp \
    track_collection.cpp \
    unique_name.cpp \
    vector.cpp \
    vector_op.cpp \
    vector_vector.cpp \
    vectorize.cpp  \
    version.cpp

