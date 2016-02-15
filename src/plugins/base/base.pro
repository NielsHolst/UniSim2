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
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
TARGET = base$${SUFFIX}

# Location of library when installed on Unix
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Our project header and source code
HEADERS +=  \
    assign.h \
    box.h \
    box_builder.h \
    box_reader_base.h \
    box_reader_boxes.h \
    box_reader_xml.h \
    box_output.h \
    box_step.h \
    command.h  \
    command_help.h  \
    convert.h \
    dialog.h \
    dialog_base.h \
    dialog_stub.h \
    environment.h \
    exception.h \
    factory_plug_in.h \
    general.h \
    history.h \
    initialize.h \
    mega_factory.h \
    object_pool.h \
    path.h \
    port.h \
    port_transform.h \
    port_type.h \
    product_base.h \
    product.h \
    random_generator.h \
    test_num.h \
    time_with_units.h \
    unique_name.h \
    vector.h \
    vector_vector.h \
    vectorize.h \
    organisation.h

SOURCES +=  \
    assign.cpp \
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
    dialog.cpp \
    dialog_base.cpp \
    dialog_stub.cpp \
    environment.cpp \
    exception.cpp \
    general.cpp \
    history.cpp \
    initialize.cpp \
    mega_factory.cpp \
    object_pool.cpp \
    path.cpp \
    port.cpp \
    port_transform.cpp \
    port_type.cpp \
    product_base.cpp \
    random_generator.cpp \
    time_with_units.cpp \
    unique_name.cpp \
    vector.cpp \
    vector_vector.cpp \
    vectorize.cpp



