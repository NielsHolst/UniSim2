/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_TOPSOILDEGRADATIONFINAL_H
#define PESTTOX_TOPSOILDEGRADATIONFINAL_H

#include <base/box.h>

namespace PestTox {

class TopsoilDegradation : public base::Box
{
public:
    TopsoilDegradation(QString name, QObject *parent);
    void update();

private:
    // Input
    double inflow, DT50, ff, Q, Tsoil, Rainfall, threshold, //timeSinceLastRainEvent,
    Vrsmc, rf, fw, Sp, Doseldw, ksv, frsa, frsw, fsr, fmp;

    // Output
    double concentration, outflow, degradation, volatilization,
    ksd, fsdT, Vsmc, fsm, Dosesr, sdDoseaflrm, Dmacropore;

};

} //namespace


#endif
