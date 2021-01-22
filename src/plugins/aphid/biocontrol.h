/* Copyright 2018-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BIOCONTROL_H
#define BIOCONTROL_H
#include <base/box.h>

namespace aphid {

class Biocontrol : public base::Box
{
public:
    Biocontrol(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double
        cadaverDensity, cropGrowthStage,
        aphidDensityWithoutF, aphidDensityWithF,
        aphidPressureWithoutF, aphidPressureWithF,
        percentageCadavers,
        yieldWithoutF, yieldWithF;
    // Outputs
    double aphidPressureDifference, yieldDifference,
        maxAphidDensityWithoutF, maxAphidDensityCropGSWithoutF,
        maxAphidDensityWithF, maxAphidDensityCropGSWithF,
        maxCadaverDensity, maxCadaverDensityCropGS, maxCadaverDensityPercentage,
        maxPercentageCadavers, maxPercentageCadaversCropGS, maxPercentageCadaversDensity,
        percentageCadaversGs43, percentageCadaversGs61, percentageCadaversGs73,
        maxPrevalence, maxPrevalenceCropGS, maxPrevalenceDensity;
};

}

#endif
