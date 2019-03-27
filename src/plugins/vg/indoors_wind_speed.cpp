/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "indoors_wind_speed.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(IndoorsWindSpeed)

IndoorsWindSpeed::IndoorsWindSpeed(QString name, QObject *parent)
	: Box(name, parent)
{
    help("models indoors wind speed");
    Input(ventilation).imports("indoors/total/airFlux[value]").unit("m3/m3/h");
    Input(constructionWidth).imports("geometry[width]").unit("m");
    Output(value).help("Indoors wind speed [m/s]").unit("m/s");
}

void IndoorsWindSpeed::reset() {
    value = 0.;
}

void IndoorsWindSpeed::update() {
    value = ventilation*constructionWidth/3600.; // m/s = h-1 * m *  h/s
    // volume/(length*height) = width
    // See Wang et al. (2000)
}

} //namespace

