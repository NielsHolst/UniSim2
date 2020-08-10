/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "vapour_flux_condensation_screen_base.h"

using std::min;
using namespace base;
using namespace phys_math;

namespace vg {

VapourFluxCondensationScreenBase::VapourFluxCondensationScreenBase(QString name, QObject *parent, int layer)
    : VapourFluxBase(name, parent)
{
    QString number = QString::number(layer);
    Input(groundArea).imports("geometry[groundArea]", CA);
    Input(screenAreas).imports("shelter/*/screens/layer"+number+"[area]", CA);
    Input(screenTemperature).imports("energyBudget/screen"+number+"[temperature]", CA);
    Input(screenedTemperature).imports("airSpaces/screened[temperature]", CA);
    Input(roomTemperature).imports("indoors[temperature]", CA);
    Input(indoorsAh).imports("indoors/humidity[ah]", CA);
}

inline double condensation(double dTemp) {
    return (dTemp > 0) ? 1.64e-3*pow(dTemp, 1/3.) : 0.; // GCC, p.147
}

void VapourFluxCondensationScreenBase::update() {
    double screenPerGroundArea = vector_op::sum(screenAreas)/groundArea,
           conductanceScreened = condensation(screenedTemperature - screenTemperature)*screenPerGroundArea,
           conductanceRoom     = condensation(    roomTemperature - screenTemperature)*screenPerGroundArea,
           surfaceSah          = sah(screenTemperature),
           vapourFluxScreened  = min(conductanceScreened*(surfaceSah - indoorsAh), 0.),
           vapourFluxRoom      = min(conductanceRoom    *(surfaceSah - indoorsAh), 0.),
           gainScreened        = conductanceScreened*surfaceSah,
           gainRoom            = conductanceScreened*surfaceSah;
    conductance = conductanceScreened + conductanceRoom;
    vapourFlux  = vapourFluxScreened  + vapourFluxRoom;
    gain        = gainScreened        + gainRoom;
}

} //namespace

