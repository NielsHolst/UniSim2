/* Copyright 2018-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PERCENTAGE_CADAVERS_H
#define PERCENTAGE_CADAVERS_H
#include <base/box.h>

namespace aphid {

class PercentageCadavers : public base::Box
{
public:
    PercentageCadavers(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double aphidDensity, cadaverDensity;
    // Outputs
    double value;
};

}

#endif
