#include <base/publish.h>
#include "Beta.h"

using namespace base;

namespace saccharina {

PUBLISH(Beta)

Beta::Beta(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the value of beta - the photoinhibition parameter");
    Input(alpha).equals(0.0000375).help("Photosynthetic efficiency");
    Input(Isat).equals(200).help("Irradiance for maximal photosynthesis");
    Input(maxphotosynthesisreftemp).equals(0.00144);
    Output(beta).help("Area of the algae at deployment (dm)");
}

void Beta::reset() {
    beta = 0.000000001;
}

void Beta::update() {
    double beta;
    beta = 0.000000001;
     for (int i=0; i<=10; ++i) {
   beta = beta - (((alpha*Isat)/log(1+alpha/beta))*(alpha/(alpha+beta))*pow((beta/(alpha+beta)),(beta/alpha))-maxphotosynthesisreftemp)/(((alpha * Isat) * (alpha/pow(beta,2)/(1 + alpha/beta))/pow(log(1 + alpha/beta),2) * (alpha/(alpha + beta)) - ((alpha * Isat)/log(1 + alpha/beta)) * (alpha/pow((alpha + beta),2))) * pow((beta/(alpha + beta)),(beta/alpha)) + ((alpha * Isat)/log(1 + alpha/beta)) * (alpha/(alpha + beta)) * (pow((beta/(alpha + beta)),((beta/alpha) - 1)) * ((beta/alpha) * (1/(alpha + beta) - beta/pow((alpha + beta),2)) + pow((beta/(alpha + beta)),(beta/alpha)) * (log((beta/(alpha + beta))) * (1/alpha)))));
}
}

}
