#include <base/publish.h>
#include "Nb_tillersD.h"

using namespace base;

namespace aphid {

PUBLISH(Nb_tillersD)

Nb_tillersD::Nb_tillersD(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Wheat_Pheno);
    Output(nb_tillers);
}

void Nb_tillersD::reset() {
    nb_tillers=0;
}

void Nb_tillersD::update() {
    nb_tillers=round(20+90.4*Wheat_Pheno-2.69*pow(Wheat_Pheno,2)+ 0.0321*pow(Wheat_Pheno,3)-0.000134*pow(Wheat_Pheno,4));
}

}
