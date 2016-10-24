/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_TOPSOILDOSEINSOILSOLIDSAIRANDWATER_H
#define PESTTOX_TOPSOILDOSEINSOILSOLIDSAIRANDWATER_H

#include <base/box.h>

namespace PestTox {

class topsoildoseinsoilsolidsairandwater : public base::Box
{
public:
    topsoildoseinsoilsolidsairandwater(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Za, Zw, Zs, Vs, Vw, Va, VZ;

    // Output
    double frss, frsw, frsa;
};

} //namespace


#endif
