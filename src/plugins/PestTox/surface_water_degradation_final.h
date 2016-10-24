/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_SURFACEWATERDEGRADATIONFINAL_H
#define PESTTOX_SURFACEWATERDEGRADATIONFINAL_H

#include <base/box.h>

namespace PestTox {

class surfacewaterdegradationfinal  : public base::Box
{
public:
    surfacewaterdegradationfinal (QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double inflow, k, ff, Q, Tref, Tair;

    // Output
    double concentration, outflow, Tw, kwd, fsdT;

};

} //namespace


#endif
