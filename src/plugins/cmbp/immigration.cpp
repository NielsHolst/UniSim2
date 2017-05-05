#include <base/publish.h>
#include "immigration.h"

using namespace base;

namespace cmbp {

PUBLISH(Immigration)

Immigration::Immigration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(number).help("Nuber of P.t. arriving");
    Input(storeMass).equals(500).help("Size of store (kg maize)");
    Input(day).help("Day of immigration (no. of days after start)");
    Output(value).help("Immigrant increase in P.t. density (per kg");
}

void Immigration::reset() {
    _day = 0;
    _happened = false;
}

void Immigration::update() {
    bool passedImmigrationDay = (_day++>=day);
    value = (passedImmigrationDay && !_happened)  ? number/storeMass : 0;
    _happened = passedImmigrationDay;
}


}
