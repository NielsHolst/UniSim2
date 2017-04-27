/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_SURFACEWATERDEGRADATION_H
#define PESTTOX_SURFACEWATERDEGRADATION_H

#include <base/box.h>

namespace PestTox {

class SurfaceWaterDegradation  : public base::Box
{
public:
    SurfaceWaterDegradation (QString name, QObject *parent);
    void update();

private:
    // Input
    double inflow, DT50, Tw, pondWidth,
    pondHeight, pondLength, inflow2,
    fieldArea, RunoffAmount, fieldWater;

    // Output
    double concentration, outflow,
    kwd, fwdT, pondVolume, runoff, sprayDrift;

};

} //namespace


#endif
