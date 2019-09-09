/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PESTTOX_PESTICIDEINDUCEDMORTALITYRATE_H
#define PESTTOX_PESTICIDEINDUCEDMORTALITYRATE_H

#include <base/box.h>

namespace PestTox {

class pesticideinducedmortalityrate : public base::Box
{
public:
    pesticideinducedmortalityrate(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double LC50, concentration, slope;

    // Output
    double mortalityRate, survivalRate;
};

} //namespace


#endif
