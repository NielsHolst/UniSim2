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
#    altova_xml.h \
    bare_date.h \
    base_signal.h \
    box.h \
    box_builder.h \
#    box_reader_xml.h \
    boxscript.h \
    boxscript_ast.h \
    boxscript_ast_adapted.h \
    boxscript_config.h \
    boxscript_def.h \
    boxscript_error_handler.h \
    boxscript_parser.h \
    boxscript_preprocessor.h \
    boxscript_skipper.h \
    caller.h \
    convert.h \
    convert_operator.h \
    circular_buffer.h \
    command.h  \
    command_help.h  \
    computation_step.h \
    construction_step.h \
#    copy_folder.h \
    data_frame.h \
#    data_grid.h \
#    date_time.h \
    dialog.h \
    dialog_base.h \
    dialog_minimal.h \
    dialog_quiet.h \
    dialog_stub.h \
    dialog_widget.h \
    documentation.h \
    environment.h \
    exception.h \
    exception_context_class.h \
    expression.h \
    factory_plug_in.h \
#    food_web_box_base.h \
    general.h \
    history.h \
    interpolate.h \
    matrix.h \
    matrix2D.h \
    mega_factory.h \
    minimise.h \
    object_pool.h \
    organisation.h \
    operate.h \
    operator.h \
    path.h \
    port.h \
    port_access.h \
    port_mode.h \
    product.h \
    product_base.h \
    physiological_time.h \
    phys_math.h \
    reader_base.h \
    reader_boxscript.h \
    proportions.h \
    psychrolib.h \
    random_generator.h \
    random_order.h \
    save_grammar_atom.h \
    save_grammar_base.h \
    save_grammar_notepad.h \
#    save_graph_base.h \
#    save_graph_graphviz.h \
    table.h \
    test_num.h \
    to_q_string_list.h \
    time_with_units.h \
    timer.h \
    unique_name.h \
    value.h \
    value_collection.h \
    value_typed.h \
#    vector.h \
    vector_op.h \
#    vector_vector.h \
#    vectorize.h \
    version.h \
    win_taskbar_progress.h

SOURCES +=  \
#    altova_xml.cpp \
    base_signal.cpp \
    bare_date.cpp \
    box.cpp \
    box_builder.cpp \
#    box_reader_xml.cpp \
    boxscript.cpp \
    boxscript_ast.cpp \
    boxscript_parser.cpp \
    boxscript_preprocessor.cpp \
    caller.cpp \
    convert.cpp \
    convert_operator.cpp \
    command.cpp \
    command_help.cpp  \
    computation_step.cpp \
    construction_step.cpp \
#    copy_folder.cpp \
    data_frame.cpp \
#    data_grid.cpp \
#    date_time.cpp \
    dialog.cpp \
    dialog_base.cpp \
    dialog_minimal.cpp \
    dialog_quiet.cpp \
    dialog_stub.cpp \
    dialog_widget.cpp \
    documentation.cpp \
    environment.cpp \
    exception.cpp \
    exception_context_class.cpp \
    expression.cpp \
    general.cpp \
    history.cpp \
    mega_factory.cpp \
    object_pool.cpp \
    operate.cpp \
    operator.cpp \
    path.cpp \
    physiological_time.cpp \
    phys_math.cpp \
    port.cpp \
    port_access.cpp \
    reader_base.cpp \
    reader_boxscript.cpp \
    product_base.cpp \
    proportions.cpp \
    psychrolib.c \
    random_generator.cpp \
    random_order.cpp \
    save_grammar_atom.cpp \
    save_grammar_base.cpp \
    save_grammar_notepad.cpp \
#    save_graph_base.cpp \
#    save_graph_graphviz.cpp \
    table.cpp \
    time_with_units.cpp \
    timer.cpp \
    unique_name.cpp \
    value.cpp \
    value_collection.cpp \
#    vector.cpp \
    vector_op.cpp \
#    vector_vector.cpp \
#    vectorize.cpp  \
    version.cpp

