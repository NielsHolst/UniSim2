#include <base/publish.h>
#include "gross_photosynthesis.h"

using namespace base;

namespace saccharina {

PUBLISH(GrossPhotosynthesis)

GrossPhotosynthesis::GrossPhotosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates gross photosynthetic rate (eqs. 10-11)");
    Input(alpha).imports("lightInhibition[alpha]");
    Input(beta).imports("lightInhibition[beta]");
    Input(I).imports("env[I]");
    Input(Isat).equals(200).unit("micromol/m2/s").help("Irradiance for maximal photosynthesis");
    Output(value).unit("g C/dm2/h").help("Gross photosynthetic rate");
}

void GrossPhotosynthesis::reset() {
   update();
}

void GrossPhotosynthesis::update() {
    double PS = alpha*Isat/log(1.+alpha/beta);
    value = PS*(1. - exp(-alpha*I/PS)) * exp(-beta/PS);
}

}
