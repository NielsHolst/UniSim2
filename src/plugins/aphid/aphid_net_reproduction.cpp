#include <cmath>
#include <base/publish.h>
#include "aphid_net_reproduction.h"

using std::min;
using namespace base;

namespace aphid {

PUBLISH(AphidNetReproduction)

AphidNetReproduction::AphidNetReproduction(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes net reproduction capacity for S. avenae");
    Input(R0opt).equals(51.6).help("Optimum net reproduction").unit("per capita");
    Input(Tmin).equals(3).help("Minimum temperature under which no reproduction occur").unit("oC");
    Input(Tmax).equals(30).help("Maximum temperature over which no reproduction occur anymore").unit("oC");
    Input(Topt).equals(16.1).help("Optimum temperature for reproduction").unit("oC");
    Input(temperature).equals(0).help("Daily average temperature").unit("oC");
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");;
    Input(aphidDensity).help("Aphid density").unit("per tiller");
    Input(aphidDensityThreshold).help("Density threshold when net reproduction is zero").unit("per tiller");
    Input(exposureCost).help("Relative reduction in reproduction when exposed").unit("[0;1]");
    Output(apterous).help("Net reproduction for apterous aphids").unit("per capita");
    Output(alate).help("Net reproduction for alate aphids").unit("per capita");
} 

void AphidNetReproduction::update() {
    // TEMPERATURE EFFECT ON LIFETIME FECUNDITY
    // From Tmin to Topt
    double a, b;
    if (temperature>=Tmin && temperature<=Topt) {
        a = R0opt/(Topt-Tmin);
        b = (-Tmin*R0opt)/(Topt-Tmin);
    }
    // From Topt to Tmax
    else if (temperature>Topt && temperature<=Tmax) {
        a = R0opt/(Topt-Tmax);
        b = (-Tmax*R0opt)/(Topt-Tmax);
    }
    // Outside Tmin and Tmax
    else {
        a = b = 0.;
    }
    apterous = a*temperature + b;

    // THE LIMIT SITUATIONS CLOSE TO Tmin AND Tmax
    if (apterous < 0.)
        apterous = 0.;

    // WHEAT PHENOLOGY EFFECT ON LIFETIME FECUNBITY
    if (cropGrowthStage>=59 && cropGrowthStage<=73)
        apterous *= 1.6;
    else if (cropGrowthStage>80)
        apterous = 0.;

    // CROWDING
    if (aphidDensityThreshold > 0.) {
        double scaling = 1. - min(aphidDensity/aphidDensityThreshold,1.);
        apterous *= scaling;
    }

    // THE CASE OF THE ALATES NOW
    // In Duffy et al. 2017 the maximum daily nymph production per alates=2/3 of the max fec of apterous
    alate = apterous*2./3.;

    // Reduction when exposed
    apterousExposed = (1.-exposureCost)*apterous;
    alateExposed = (1.-exposureCost)*alate;
}


}
