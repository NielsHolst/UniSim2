#include <base/publish.h>
#include "spo_cadavers.h"

using namespace base;

namespace aphid {

PUBLISH(Spo_cadavers)

Spo_cadavers::Spo_cadavers(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_cadavers_ready);
    Input (Ticking);
    Output(nb_spo_cadavers);
}

void Spo_cadavers::reset() {
    Ticking=false;
    nb_spo_cadavers=0;
}

void Spo_cadavers::update() {
    if(Ticking) {nb_spo_cadavers=nb_cadavers_ready;}
    else{nb_spo_cadavers=0;}
}
}
