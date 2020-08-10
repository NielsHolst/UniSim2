/* Copyright 2018-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "percentage_cadavers.h"

using namespace base;

namespace aphid {

PUBLISH(PercentageCadavers)

PercentageCadavers::PercentageCadavers(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes percentage of cadavers");
    Input(aphidDensity).help("Current density of aphids").unit("per tiller");
    Input(cadaverDensity).help("Current density of cadavers").unit("per tiller");
    Output(value).help("Percentage cadavers out of aphids+cadavers").unit("[0;1]");
}

void PercentageCadavers::update() {
    double total = aphidDensity + cadaverDensity,
           percentage = (total > 0.) ? cadaverDensity/total : 0.;
    if (percentage > value) {
        value = percentage;
    }
}

}
