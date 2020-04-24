/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <base/data_grid.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/interpolate.h>
#include <base/publish.h>
#include "cover.h"

using namespace base;

namespace vg {

PUBLISH(Cover)

Cover::Cover(QString name, QObject *parent)
    : RadiationLayer(name, parent)
{
    help("computes cover light and heat characteristics");
    Input(windSpeed).imports("outdoors[windSpeed]",CA).unit("m/s");
    Input(haze).equals(1.).help("Proportion of direct light transmitted as diffuse light").unit("[0;1]");
    Input(UwindMinimum).equals(7.2).help("Heat transfer coefficient at no wind").unit("W/m2/K");
    Input(UwindSlope).help("Heat transfer coefficient linear increase with wind speed").unit("W/m2/K/(m/s)");
}

void Cover::initialize() {
    // Default to glass
    swReflectivityTop    = lwReflectivityTop =
    swReflectivityBottom = lwReflectivityBottom = 0.1;
    swTransmissivityTop  = lwTransmissivityBottom = 0.9;
    Ubottom =  1.247;
    specificHeatCapacity = 6968.;
    temperature = 20.;
    updateAbsorptivities();
}

void Cover::reset() {
    update();
}

void Cover::update() {
    updateAbsorptivities();
    Utop = UwindMinimum + windSpeed*UwindSlope;
//    heatCapacity = specificHeatCapacity*area;
}

} //namespace

