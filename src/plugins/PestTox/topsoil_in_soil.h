/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_TOPSOILDOSEINSOILSOLIDSAIRANDWATER_H
#define PESTTOX_TOPSOILDOSEINSOILSOLIDSAIRANDWATER_H

#include <base/box.h>

namespace PestTox {

class TopsoilInSoil : public base::Box
{
public:
    TopsoilInSoil(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Z, V, VZ;

    // Output
    double fr;
};

} //namespace


#endif
