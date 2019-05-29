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
    box.h \
    box_builder.h \
    box_reader_base.h \
    box_reader_boxes.h \
    box_reader_xml.h \
    box_output.h \
    circular_buffer.h \
    command.h  \
    command_help.h  \
    computation_step.h \
    construction_step.h \
    convert.h \
    copy_folder.h \
    data_frame.h \
    data_grid.h \
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
    factory_plug_in.h \
    food_web_box_base.h \
    general.h \
    history.h \
    initialize.h \
    interpolate.h \
    matrix.h \
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
    port_type.h \
    port_value_op.h \
    product_base.h \
    product.h \
#    progress_indicator_base.h \
#    progress_indicator_progress_bar.h \
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
    version.h

SOURCES +=  \
    altova_xml.cpp \
    any_year.cpp \
    assign.cpp \
    ast_boxes.cpp \
    ast_common.cpp \
    box.cpp \
    box_builder.cpp \
    box_output.cpp \
    box_reader_base.cpp \
    box_reader_boxes.cpp \
    box_reader_xml.cpp \
    command.cpp \
    command_help.cpp  \
    computation_step.cpp \
    construction_step.cpp \
    convert.cpp \
    copy_folder.cpp \
    data_frame.cpp \
    data_grid.cpp \
    dialog.cpp \
    dialog_base.cpp \
    dialog_minimal.cpp \
    dialog_quiet.cpp \
    dialog_stub.cpp \
    dialog_widget.cpp \
    distribution.cpp \
    environment.cpp \
    exception.cpp \
    exception_context_class.cpp \
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
    port_type.cpp \
    port_value_op.cpp \
    product_base.cpp \
#    progress_indicator_base.cpp \
#    progress_indicator_progress_bar.cpp \
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

