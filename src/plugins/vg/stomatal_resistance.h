/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_STOMATAL_RESISTANCE_H
#define VG_STOMATAL_RESISTANCE_H

#include <base/box.h>

namespace vg {

class StomatalResistance : public base::Box
{
public:
    StomatalResistance(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double rh, rbCO2, co2, Pn, lai;
    // Outputs
    double rsH2O, rsCO2;
    // Methods
    void updateFromPhosyntheticRate(double A);
};
} //namespace


#endif
