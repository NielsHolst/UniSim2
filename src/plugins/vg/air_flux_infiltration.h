/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_FLUX_INFILTRATION_H
#define AIR_FLUX_INFILTRATION_H

#include <base/box.h>

namespace vg {

class AirFluxInfiltration : public base::Box
{
public:
    AirFluxInfiltration(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double leakage, windSpeed;

    // Variables
    double value;

};
} //namespace


#endif
