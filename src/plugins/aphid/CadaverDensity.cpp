#include <base/publish.h>
#include "CadaverDensity.h"

using namespace base;

namespace aphid {

PUBLISH(CadaverDensity)

CadaverDensity::CadaverDensity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_cadavers);
    Input (nb_plant);
    Output(d_cadaver); // total density, all biological stages taken into account
}

void CadaverDensity::reset() {
    d_cadaver= 0;
}

void CadaverDensity::update() {
    d_cadaver=nb_cadavers/nb_plant;
}


}
