#-------------------------------------------------
#
# boxes DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = boxes

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

# What we are building
TEMPLATE = lib
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
TARGET = boxes$${SUFFIX}

# Location of library when installed on Unix
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Our project header and source code
HEADERS +=  \
    box.h \
    box_builder.h \
    box_output.h \
    exception.h \
    factory_plug_in.h \
    general.h \
    mega_factory.h \
    object_pool.h \
    path.h \
#    path_directive.h \
    port.h \
    product_base.h \
    product.h

SOURCES +=  \
    box.cpp \
    box_builder.cpp \
    box_output.cpp \
    exception.cpp \
    general.cpp \
    mega_factory.cpp \
    object_pool.cpp \
    path.cpp \
#    path_directive.cpp \
    port.cpp \
    product_base.cpp


