#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = MusselBed

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS += \
    mussel.h \
    mussel_growth_rate.h \
    starfish.h \
    starfish_search_rate.h \
    starfish_demand.h \
    mussel_sal_scale.h \
    mussel_temp_scale.h \
    starfish_hd_scale.h \
    starfish_sal_scale.h \
    starfish_temp_scale.h \
    mussel_thinning.h

SOURCES += \
    mussel.cpp \
    mussel_growth_rate.cpp \
    starfish.cpp \
    starfish_search_rate.cpp \
    starfish_demand.cpp \
    mussel_sal_scale.cpp \
    mussel_temp_scale.cpp \
    starfish_hd_scale.cpp \
    starfish_sal_scale.cpp \
    starfish_temp_scale.cpp \
    mussel_thinning.cpp


