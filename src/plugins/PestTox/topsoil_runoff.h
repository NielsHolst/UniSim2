/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_TOPSOILRUNOFF_H
#define PESTTOX_TOPSOILRUNOFF_H

#include <base/box.h>

namespace PestTox {

class TopSoilRunoff : public base::Box
{
public:
    TopSoilRunoff(QString name, QObject *parent);
    void update();

private:
    // Input
    double S, wbz, Doseaflrm, Doseldw, P, KocpH, fom, Q;

    // Output
    double Fslope, Fbuffer, F, fsr, Kd;
};

} //namespace


#endif
