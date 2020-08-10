/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "ventilation_net.h"

using namespace base;

namespace vg {

PUBLISH(VentilationNet)

VentilationNet::VentilationNet(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes air flux from outdoors to indoors compartments");
    Input(ventilationTotal).imports("ventilation/total[value]", CA);
    Input(timeStep).imports("calendar[timeStepSecs]", CA);
    Input(roomVolume).imports("geometry[roomVolume]", CA);
    Input(screenedVolume).imports("geometry[screenedVolume]", CA);
    Input(airTransmissivities).imports("shelter/*/screens/airTransmissivity[value]", CA);
    Input(areas).imports("shelter/*/area[value]", CA);
    Output(roomProp).help("Proportion of air moved from outdoors to room").unit("[0;1]");
    Output(screenedProp).help("Proportion of air moved from outdoors to screened volume").unit("[0;1]");
    Output(room).help("Volume of air moved from outdoors to room").unit("m3");
    Output(screened).help("Volume of air moved from outdoors to screened volume").unit("m3");
}

void VentilationNet::reset() {
    update();
}

void VentilationNet::update() {
    double airTransmissivity = vector_op::weightedAverage(airTransmissivities, areas, this);
    roomProp     = 1. - exp(-ventilationTotal*timeStep/3600.*airTransmissivity);
    screenedProp = 1. - exp(-ventilationTotal*timeStep/3600.);
    room     = roomVolume    *roomProp;
    screened = screenedVolume*screenedProp;
}

} //namespace

