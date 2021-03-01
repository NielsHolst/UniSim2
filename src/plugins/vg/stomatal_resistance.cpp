/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "general.h"
#include <base/publish.h>
#include "stomatal_resistance.h"

using std::max;
using namespace base;

namespace vg {

PUBLISH(StomatalResistance)

/*! \class StomatalResistance
 * \brief Stomatal resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _co2_ is the ambient CO<SUB>2</SUB> concentration [ppm]
 * - _rh_ is indoors relative humidity [0;100]
 * - _Pg_ is gross assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the stomatal resistance against water vapour [s/m]
 * - _rbCO2_ is the stomatal resistance against CO<SUB>2</SUB> [s/m]
 *
 * Default dependencies
 * ------------
 * - an _indoors/co2_ model with a _ppm_ port [ppm]
 * - an _indoors/humidity_ model with an _rh_ port [0;100]");
 * - a _photosynthesis_ sibling model with a _Pg_ port [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - an _rb_ sibling model with an _rbCO2_ port [s/m]
 */

StomatalResistance::StomatalResistance(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(co2, "indoors/co2[value]");
    Input(rh, "indoors/humidity[rh]");
    Input(Pn, "../photosynthesis[Pn]");
    Input(rbCO2, "../rb[rbCO2]");
    Input(lai, "crop/lai[value]");
    Output(rsH2O);
    Output(rsCO2);
}

void StomatalResistance::reset() {
    updateFromPhosyntheticRate(0.);
}

void StomatalResistance::update() {
    updateFromPhosyntheticRate(Pn);
}

void StomatalResistance::updateFromPhosyntheticRate(double A) {
    const double b = 0.0960;
    const double m = 10.055;
    const double Pa = P0/1e5; // 1 bar

    double Am = max(A/44/1e-6/3600, 0.); // micromole m-2 s-1
    Am *= lai;                           // convert from m2 ground to m2 leaf area
    // Internal resistance acc. Kim Lieth is in mol m-2 s-1 and recalc. with 0.025 (Jones, p. 56)
    double cs = co2 - Am*(rbCO2*0.025)*Pa;
    double giH2O = b + m*Am*rh/100/(cs/Pa);
    rsH2O = 1./giH2O/0.025;
    rsCO2 = rsH2O*1.6;
}


} //namespace

