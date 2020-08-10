#include <base/publish.h>
#include "Gross_photosynthesis.h"

using namespace base;

namespace saccharina {

PUBLISH(Grossphotosynthesis)

Grossphotosynthesis::Grossphotosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the gross photosynthesis (Eq 10)");
    Input(PS).equals(0.00144).help("Photosynthesis parameter, auxiliary variable");
    Input(alpha).equals(0.0000375).help("Photosynthetic efficiency (gCdm−2 h−1(μmol photons m−2 s−1)−1)");
    Input(I).equals(200).help("Irradiance (PAR), environmental variable");
    Input(beta).equals(0.000000001).help("Photoinhibition parameter, auxiliary variable (gO2 dm−2 h−1(μmol photons m−2 s−1)−1)");
    Output(Gphotosynthesis).help("Gross photosynthesis (gC dm−2 h−1)");
}

void Grossphotosynthesis::reset() {
   update();
}

void Grossphotosynthesis::update() {
    Gphotosynthesis = PS * (1 - exp(-((alpha * I)/(PS)))) * exp(-(beta/PS));
}
}
