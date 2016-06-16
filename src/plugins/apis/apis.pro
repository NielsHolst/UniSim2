#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = apis

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    energy_budget.h \
    honey_demand_growth.h \
    honey_demand_respiration.h \
    honey_store.h \
    honey_supply.h \
    life_stage.h \
    nectar_flow.h
	
SOURCES +=  \
    energy_budget.cpp \
    honey_demand_growth.cpp \
    honey_demand_respiration.cpp \
    honey_store.cpp \
    honey_supply.cpp \
    life_stage.cpp \
    nectar_flow.cpp

