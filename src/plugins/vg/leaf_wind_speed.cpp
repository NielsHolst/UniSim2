/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "leaf_wind_speed.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(LeafWindSpeed)

LeafWindSpeed::LeafWindSpeed(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes leaf wind speed");
    Input(k).help("Extinction coefficient for wind down throught the canopy");
    Input(indoorsWindSpeedMinimum).equals(0.025).help("Minimum indoors wind speed [m/s]");
    Input(indoorsWindSpeed).imports("indoors/windSpeed[value]");
    Input(xGauss).imports("..[xGaussUpperside]");
    Input(lai).imports("crop/lai[value]");
    Output(value).help("Wind speed at leaf level [m/s]");
}

void LeafWindSpeed::reset() {
    value = indoorsWindSpeedMinimum;
}

void LeafWindSpeed::update() {
    value = max(indoorsWindSpeed, indoorsWindSpeedMinimum)*exp(-k*lai*xGauss);
}


} //namespace

