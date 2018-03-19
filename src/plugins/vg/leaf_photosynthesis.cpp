/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "general.h"
#include "leaf_photosynthesis.h"
#include <base/publish.h>

using namespace base;
using namespace std;

namespace vg {
	
PUBLISH(LeafPhotosynthesis)

/* Canopy height (H) is arbitrarily set to 2 m.
 * This will only have an effect, in case Gaussian integration is carried out over canopy height,
 * rather than over LAI (current implementation).
*/
const double H{2};

// Leaf area density (m2/m3) at the Gausian point; constant for a uniform vertical LAI distribution
double LeafPhotosynthesis::lad() const {
//    double h = xGauss*H;
//    return lai*6*h*(H-h)/p3(H);
    return lai/H;
}

// LAI above the Gaussian point (xGauss)
double LeafPhotosynthesis::laic() const {
//    double h = xGauss*H;
//    return lai*( 1 - 1/p3(H)*p2(h)*(3*H - 2*h) );
    return lai*xGauss;
}

LeafPhotosynthesis::LeafPhotosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes light capture and photosynthetic rate");
    Input(parDiffuse).imports("indoors/light[parDiffuse]");
    Input(parDirect).imports("indoors/light[parDirect]");
    Input(kDiffuse).imports("crop/radiation[kDiffuse]");
    Input(kDirect).imports("crop/radiation[kDirect]");
    Input(kDirectDirect).imports("crop/radiation[kDirectDirect]");
    Input(scattering).imports("crop/radiation[scattering]");
    Input(diffuseReflectivity).imports("crop/radiation[diffuseReflectivity]");
    Input(directReflectivity).imports("crop/radiation[directReflectivity]");
    Input(lai).imports("crop/lai[value]");
    Input(sinb).imports("calendar[sinb]");

    Input(LUE).imports("./lightResponse[LUE]");
    Input(Pgmax).imports("./lightResponse[Pgmax]");
    Input(Rd).imports("./lightResponse[Rd]");
    Input(xGauss).imports("..[xGaussUpperside]");
    Input(wGauss).imports("..[wGaussUpperside]");

    Output(absorptivity).help("Proportion of indoors light captured by this leaf layer [0;1]");
    Output(parAbsorbed).help("Proportion of indoors PAR captured by this leaf layer [0;1]");
    Output(Pn).help("Net assimilation rate [g CO2/ground m2/h]");
    Output(Pg).help("Gross assimilation rate [g CO2/ground m2/h]");
}

void LeafPhotosynthesis::reset() {
    absorptivity = parAbsorbed = Pn = Pg = 0;
}

void LeafPhotosynthesis::update() {
    // Compute light absorned and gross assimilation
    double absorbedShaded = absorbedByShadedLeaves(),           // [J / m2 leaf / s]
           PgShade = grossAssimilation(absorbedShaded);         // [mg CO2 / m2 leaf / s]

    auto sunlit = absorbedBySunlitLeaves(absorbedShaded);
    double absorbedSunlit = sunlit.first,                       // [J / m2 leaf / s]
           PgSunlit = sunlit.second,                            // [mg CO2 / m2 leaf / s]
           propSunlit = exp(-kDirectDirect*laic()),
           absorbedTotal = (propSunlit*absorbedSunlit + (1-propSunlit)*absorbedShaded),
           PgTotal = propSunlit*PgSunlit + (1-propSunlit)*PgShade,
           integrationWeight = lad()*wGauss*H;

    // Apply integration weight and convert

    // [J / m2 ground / s] = [J / m2 leaf / s * m2 leaf / m2 ground]
//    parAbsorbed = absorbedTotal*wGauss*lai;
    parAbsorbed = absorbedTotal*integrationWeight;
    absorptivity = div0(parAbsorbed, parDiffuse + parDirect);

    // [mg CO2 / m2 ground / s] = [mg CO2 / m2 leaf / s * m2 leaf / m2 ground]
    Pg = PgTotal*integrationWeight;
    Pn = Pg - Rd*integrationWeight;

    // Convert to [g CO2 m-2 h-1]
    Pg *= 3.6;
    Pn *= 3.6;
}

double LeafPhotosynthesis::absorbedByShadedLeaves() const {
    double laic_ = laic();
    double absorbedDiffuse = (1-diffuseReflectivity)*parDiffuse*kDiffuse*exp(-kDiffuse*laic_),
           absorbedTotal = (1-directReflectivity)*parDirect*kDirect*exp(-kDirect*laic_),
           absorbedDirect = (1-scattering)*parDirect*kDirectDirect*exp(-kDirectDirect*laic_);
    return absorbedDiffuse + absorbedTotal - absorbedDirect; // [J/m2/leaf/s]
}

QPair<double, double> LeafPhotosynthesis::absorbedBySunlitLeaves(double absorbedShaded) const {
    if (Pgmax==0 || sinb==0) return qMakePair(0.,0.);

    // Direct flux absorbed by leaves perpendicular on direct beam (VISpp)[J*m-2 leaf s-1]
    // Original eq. changed to guard against sinB->0 yielding absorptivity>1
    double absorptivity = min((1-scattering)/sinb, 1.);
    double absorbedPerpendicular = absorptivity*parDirect;
    // Integration over all leaf angles
    double assimilationSum{0}, absorbedSum{0};
    for(int i=0; i<3; ++i) {
        // Total absorbed flux of sunlit leaves [J*m-2 leaf s-1]
        double absorbedSunlit = absorbedShaded + absorbedPerpendicular*xGauss3[i];
        absorbedSum += wGauss3[i]*absorbedSunlit;
        // Gross assimilation of sunlit leaves [mg CO2in*m-2 leaf s-1]
        assimilationSum += wGauss3[i]*grossAssimilation(absorbedSunlit);
    }
    return qMakePair(absorbedSum, assimilationSum);
}


double LeafPhotosynthesis::grossAssimilation(double absorbed) const {
    return (Pgmax==0) ? 0 : Pgmax*(1-exp(-absorbed*LUE/Pgmax));
}

} //namespace

