/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    double S, wbz, P, KocpH, fom, Q;

    // Output
    double Fslope, Fbuffer, F, fsr, Kd;
};

} //namespace


#endif
