/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "ventilated_latent_heat_converter.h"
#include "general.h"
#include <base/publish.h>

using namespace base;
using std::max;

namespace vg {

PUBLISH(VentilatedLatentHeatConverter)

VentilatedLatentHeatConverter::VentilatedLatentHeatConverter(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("models a latent heat converter used for humidity reduction");
    Input(groundArea).imports("geometry[groundArea]");
    Input(indoorsAh).imports("indoors/humidity[ah]");
    Input(volumeFlowRate).equals(2.5).help("Rate of air flow through the converter [m3/s]");
    // Additional outputs are needed to specify the flux of latent heat gain [W/m2]
}

void VentilatedLatentHeatConverter::update() {
    conductance = volumeFlowRate/groundArea;         // m/s = m3/s  / m2
    vapourFlux = conductance*(indoorsAh-convertedAh());    // kg/m2/s = m/s * kg/m3
    gain = conductance*convertedAh();              // kg/m2/s = m/s * kg/m3
}

double VentilatedLatentHeatConverter::dewRate() {
    // NB. Needs code for interpolated value...
    return 0.004; // kg/s
}

double VentilatedLatentHeatConverter::convertedAh() {
    return indoorsAh - dewRate()/volumeFlowRate; // kg/m3 = kg/s / m3/s
}

} //namespace

