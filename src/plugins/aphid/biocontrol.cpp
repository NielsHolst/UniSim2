/* Copyright 2018-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "biocontrol.h"

using namespace base;

namespace aphid {

PUBLISH(Biocontrol)

Biocontrol::Biocontrol(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimates biocontrol effects");
    Input(cadaverDensity).help("Current density of cadavers").unit("per tiller");
    Input(cropGrowthStage).help("Current crop growth stage").unit("Zadoks");
    Input(aphidPressureWithoutF).help("Accumulated aphid pressure without fungus").unit("aphid-days");
    Input(aphidPressureWithF).help("Accumulated aphid pressure with fungus").unit("aphid-days");
    Input(yieldWithoutF).help("Relative yield without fungus").unit("[0;1]");
    Input(yieldWithF).help("Relative yield witt fungus").unit("[0;1]");
    Output(maxCadaverDensity).help("Maximum cadaver density").unit("per tiller");
    Output(maxCadaverDensityCropGS).help("Crop growth stage at maximum cadaver density").unit("Zadoks");
    Output(aphidPressureDifference).help("Difference in aphid pressure caused by fungus").unit("aphid-days");
    Output(yieldDifference).help("Improvement in relative yield when controlled").unit("[0;1]");
}

void Biocontrol::update() {
    if (cadaverDensity > maxCadaverDensity) {
        maxCadaverDensity = cadaverDensity;
        maxCadaverDensityCropGS = cropGrowthStage;
    }
    aphidPressureDifference = aphidPressureWithoutF - aphidPressureWithF;
    yieldDifference = yieldWithF - yieldWithoutF;
}

}
