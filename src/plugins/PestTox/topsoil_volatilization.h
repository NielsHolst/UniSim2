/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_TOPSOILVOLATILIZATION_H
#define PESTTOX_TOPSOILVOLATILIZATION_H

#include <base/box.h>

namespace PestTox {

class TopsoilVolatilization : public base::Box
{
public:
    TopsoilVolatilization(QString name, QObject *parent);
    void update();

private:
    // Input
    double Tair, Pd,
    MV, BLair, fa, fw, farea, Pair,
    Za, Zw, VZ, MW, viscosity;

    // Output
    double Ba, Dw, Dv, Dbl, Bea, Bew, Da, ksv,
    Bw, Dpl, ke;
};

} //namespace


#endif
