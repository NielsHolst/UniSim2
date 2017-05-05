#include <base/publish.h>
#include "emigration.h"

using namespace base;

namespace cmbp {

PUBLISH(Emigration)

Emigration::Emigration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(e2).help("Regression parameter");
    Input(e3).help("Regression parameter");
    Input(N).help("Adult density");
    Output(value).help("Peak curve");
}

void Emigration::reset() {
    update();
}

void Emigration::update() {
    value = e2*pow(N, e3);
}


}
