#include <base/publish.h>
#include "Maximal_photosynthetic_rate.h"

using namespace base;

namespace saccharina {

PUBLISH(Maximalphotosyntheticrate)

Maximalphotosyntheticrate::Maximalphotosyntheticrate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the maximal photosynthetic rate (Eq 12)");
    Input(alpha).equals(0.0000375).help("Photosynthetic efficiency");
    Input(Isat).equals(200).help("Irradiance for maximal photosynthesis");
    Input(beta).equals(0.01).help("Photoinhibition parameter, auxiliary variable");
    Output(maxphotosynthesis).help("Maximal photosynthesis rate");
}

void Maximalphotosyntheticrate::reset() {
   update();
}

void Maximalphotosyntheticrate::update() {
    maxphotosynthesis = (alpha * Isat)/log(1+alpha/beta) * (alpha/(alpha + beta))* pow((alpha/(alpha + beta)),(beta/alpha));
}
}
