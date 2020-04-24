#include <base/publish.h>
#include "Photosynthesis.h"

using namespace base;

namespace saccharina {

PUBLISH(Photosynthesis)

Photosynthesis::Photosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the photosynthesis rate (Eq 11)");
    Input(alpha).equals(0.0000375).help("Photosynthetic efficiency");
    Input(Isat).equals(200).help("Irradiance for maximal photosynthesis");
    Input(beta).equals(0.000000001).help("Photoinhibition parameter, auxiliary variable");
    Output(photosynthesis).help("Photosynthesis rate (gO2 dm−2 h−1)");
}

void Photosynthesis::reset() {
   update();
}

void Photosynthesis::update() {
    photosynthesis = (alpha * Isat)/log(1+alpha/beta);
}
}
