/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "pet.h"

using namespace base;
using namespace phys_math;
using namespace std;

namespace coffee {

PUBLISH(Pet)

Pet::Pet(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes potential evapotranspiration");
    Input(globRad).unit("MJ/m2/d").help("Sunlight intensity");
    Input(lai).unit("m2/m2").help("Leaf area index");
    Input(interceptedRain).unit("mm/d").help("Rain intercepted by the plant");
    Input(temperature).imports("Tavg[value]");
    Input(vp).imports("weather[VP]");
    Input(windSpeed).imports("weather[WindSpeed]");
    Input(KEXT).imports("param[kExt]").help("Light extinction coefficient of coffee");
    Input(KEXTT).imports("param[kExtT]").help("Light extinction coefficient of crown");
    Output(potEvaporation).unit("mm/d").help("Potential evaporation");
    Output(potTranspiration).unit("mm/d").help("Potential transpiration");
}

void Pet::update() {
    const double Psych = 0.067; // Psychrometric constant (kPa/K)
    double
        // Thermal radiation (W/m2/K4 * K4 * s/d = J/m2/d)
        bbRad = Sigma*p4K(temperature)*86400,

        // Physical approximations
        svp = 0.611*exp(17.4*temperature/(temperature + 239)), // Saturated water vapour pressure (kPa)
        svpSlope = 4158.6*svp/p2(temperature + 239), // Slope between SVP and temperature (kPa/K)

        /* Net outgoing longwave radiation (J/m2/d).
        The net energy flux leaving the earth's surface is less than that emitted.
        Water vapor, clouds, carbon dioxide and dust are absorbers and emitters of longwave radiation.
        This equation approximates this by a linear relation on relative humidity (vp/svp) */
        radOut = bbRad*max(0., 0.55*(1-vp/svp)),

        // Net radiation hitting soil and coffee with reflectances 0.15 and 0.25 (J/m2/d)
        netRadSoil   = globRad*1e6*(1. - 0.15) - radOut,
        netRadCoffee = globRad*1e6*(1. - 0.25) - radOut,

        // Psi-corrected net radiation (J/m2/d)
        psi = svpSlope/(svpSlope + Psych),
        penmRadSoil = netRadSoil*psi,
        penmRadCoffee = netRadCoffee*psi,

        // Heat transfer by convection
        windFactor = 2.63*(1 + 0.54*windSpeed), // (kg/m2/d/kPa)
        penmD = LHe*windFactor*(svp-vp)*Psych/(svpSlope + Psych); // (J/m2/d)

    double totalPetSoil   = (penmRadSoil   + penmD)/LHe,
           totalPetCoffee = (penmRadCoffee + penmD)/LHe,
           propEvap       = exp(-0.5*lai);
    potEvaporation = propEvap*totalPetSoil;
    potTranspiration = max(0., (1. - propEvap)*totalPetCoffee - 0.5*interceptedRain);
}

} //namespace

