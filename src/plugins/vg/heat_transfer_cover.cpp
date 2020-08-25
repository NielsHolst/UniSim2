/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "heat_transfer_cover.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferCover)

HeatTransferCover::HeatTransferCover(QString name, QObject *parent)
    : HeatTransferShelter(name, parent, "shelter/*/cover")
{
    help("holds the radiative parameters of the greenhouse cover averaged for all shelter faces");
    port("condensationRate")->imports("waterBudget/condensationCover[vapourFlux]");
}

void HeatTransferCover::update() {
    updateLwEmission();
    updateConvectiveProperties();
    updateTemperature();
}

} //namespace

