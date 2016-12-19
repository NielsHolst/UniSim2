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
#CONFIG -= lib_bundle
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
TARGET = universal_simulator_base$${SUFFIX}

# Our project header and source code
HEADERS +=  \
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
    box_step.h \
    circular_buffer.h \
    command.h  \
    command_help.h  \
    convert.h \
    copy_folder.h \
    data_grid.h \
    dialog.h \
    dialog_base.h \
    dialog_stub.h \
    distribution.h \
    environment.h \
    exception.h \
    factory_plug_in.h \
    general.h \
    history.h \
    initialize.h \
    interpolate.h \
    mega_factory.h \
    object_pool.h \
    organisation.h \
    path.h \
    port.h \
    port_buffer.h \
    port_transform.h \
    port_type.h \
    product_base.h \
    product.h \
    random_generator.h \
    save_grammar_atom.h \
    save_grammar_base.h \
    save_grammar_notepad.h \
    save_graph_base.h \
    save_graph_graphviz.h \
    test_num.h \
    time_with_units.h \
    timer.h \
    unique_name.h \
    vector.h \
    vector_op.h \
    vector_vector.h \
    vectorize.h \
    version.h \
    enum_functions.h

SOURCES +=  \
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
    box_step.cpp \
    command.cpp \
    command_help.cpp  \
    convert.cpp \
    copy_folder.cpp \
    data_grid.cpp \
    dialog.cpp \
    dialog_base.cpp \
    dialog_stub.cpp \
    distribution.cpp \
    environment.cpp \
    exception.cpp \
    general.cpp \
    history.cpp \
    initialize.cpp \
    mega_factory.cpp \
    object_pool.cpp \
    path.cpp \
    port.cpp \
    port_buffer.cpp \
    port_transform.cpp \
    port_type.cpp \
    product_base.cpp \
    random_generator.cpp \
    save_grammar_atom.cpp \
    save_grammar_base.cpp \
    save_grammar_notepad.cpp \
    save_graph_base.cpp \
    save_graph_graphviz.cpp \
    time_with_units.cpp \
    timer.cpp \
    unique_name.cpp \
    vector.cpp \
    vector_op.cpp \
    vector_vector.cpp \
    vectorize.cpp  \
    version.cpp

