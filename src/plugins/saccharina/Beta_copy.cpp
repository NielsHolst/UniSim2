#include <base/publish.h>
#include "Beta_copy.h"

using namespace base;

namespace saccharina {

PUBLISH(Beta_copy)

Beta_copy::Beta_copy(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the value of beta - the photoinhibition parameter");
    Input(alpha).equals(0.0000375).help("Photosynthetic efficiency");
    Input(Isat).equals(200).help("Irradiance for maximal photosynthesis");
    Input(maxphotosynthesisreftemp).equals(0.00144);
    Input(beta1).equals(1);
    Output(beta_copy).help("Area of the algae at deployment (dm)");
}

void Beta_copy::reset() {
}

void Beta_copy::update() {
     for (int beta_copy=1; beta_copy<10; beta_copy++) {
   beta_copy = beta_copy - beta_copy+1;
}
}

}
