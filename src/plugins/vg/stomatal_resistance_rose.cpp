/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "stomatal_resistance_rose.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(StomatalResistanceRose)

StomatalResistanceRose::StomatalResistanceRose(QString name, QObject *parent)
    : StomatalResistanceBase(name, parent)
{
    help("computes stomatal resistance of rose");
    Input(co2).imports("indoors/co2[value]",CA).unit("ppm");
    Input(rh).imports("indoors/humidity[rh]",CA).unit("[0;100]");
    Input(Pn).imports("../photosynthesis[Pn]",CA).unit("g CO2/ground m2/h");
    Input(rbCO2).imports("../rb[rbCo2]",CA).unit("s/m");
    Input(lai).imports("..[lai]",CA).unit("m2/m2");
}

double StomatalResistanceRose::resetRsH2O() {
    return calcRsH2O(0);
}

double StomatalResistanceRose::updateRsH2O() {
    return calcRsH2O(Pn);
}

double StomatalResistanceRose::calcRsH2O(double A) {
    const double b = 0.0960;
    const double m = 10.055;
    double Am = max(A/44/1e-6/3600, 0.); // micromole m-2 s-1
    Am *= lai;     // convert from m2 ground to m2 leaf area
    // Atmospheric pressure
    double Pa = P0/1e5; // bar
    // Internal resistance acc. Kim Lieth is in mol m-2 s-1 and recalc. with 0.025 (Jones, p. 56)
    double cs = max(co2 - Am*(rbCO2*0.025)*Pa, 0.);
    double giH2O = b + m*Am*rh/100/(cs/Pa);
    double rs = 1./giH2O/0.025;
    if (std::isnan(rs))
        ThrowException("Stomatal resistance is not a number").context(this);
    return rs;
}


} //namespace

