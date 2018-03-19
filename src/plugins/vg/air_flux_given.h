/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_FLUX_GIVEN_H
#define AIR_FLUX_GIVEN_H

#include <base/box.h>

namespace vg {

class AirFluxGiven : public base::Box
{
public:
    AirFluxGiven(QString name, QObject *parent);
    void amend();
    void update();
private:
    // Inputs
    QVector<double> airFluxes;
    // Outputs
    double value;
};
} //namespace


#endif
