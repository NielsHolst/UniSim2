/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include <base/publish.h>
#include "leaf_transpiration.h"
using std::max;

using namespace base;

namespace vg {

PUBLISH(LeafTranspiration)

/*! \class LeafTranspiration
 * \brief Transpiration from a canopy layer
 *
 * Inputs
 * ------
 * - _lai_ is leaf area index [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _fractionPlantArea_ is the fraction of the ground area covered by the plant culture [0;1]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _radiationAbsorbed_ is the absorbed radiation [W/m<SUP>2</SUP>]
 * - _leafTemperature_ is leaf temperature [<SUP>o</SUP>C]
 * - _rbH2O_ is the boundary layer resistance to water vapour [s/m]
 * - _rsH2O_ is the internal leaf resistance to water vapour [s/m]
 *
 * Outputs
 * ------
 * - _leafAh_ is leaf absolute humidity [kg/m<SUP>3</SUP>]
 * - see also VapourFluxBase
 */

LeafTranspiration::LeafTranspiration(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    Input(lai, "crop/lai[value]");
    Input(fractionPlantArea, "crop/lai[fractionPlantArea]");
    Input(indoorsAh, "indoors/humidity[ah]");
    Input(radiationAbsorbed, "../radiationAbsorbed[value]");
    Input(leafTemperature, "../Temperature[value]");
    Input(rbH2O, "../rb[rbH2O]");
    Input(rsH2O, "../rs[rsH2O]");
    Output(leafAh);
}

void LeafTranspiration::reset() {
    VapourFluxBase::reset();
    leafAh = indoorsAh;
}

void LeafTranspiration::update() {
    if (lai == 0.) {
        conductance = vapourFlux = gain = 0.;
    }
    else {
        double laiNet = lai*fractionPlantArea,
               s = svpSlope(leafTemperature);
        conductance = 2*laiNet/((1 + s/Psychr)*rbH2O + rsH2O);
        leafAh = sah(leafTemperature) + s/Psychr*rbH2O/2/laiNet*radiationAbsorbed/LHe;
        vapourFlux = max(conductance*(leafAh - indoorsAh), 0.);
        gain = conductance*leafAh;
    }
}


} //namespace

