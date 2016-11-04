/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_LEAFWASHOFF_H
#define PESTTOX_LEAFWASHOFF_H

#include <base/box.h>

namespace PestTox {

class LeafWashOff : public base::Box
{
public:
    LeafWashOff(QString name, QObject *parent);
    void update();

private:
    // Input
    double P, woc, inflow, kov;

    // Output
    double value, outflow, concentration;
};

} //namespace


#endif
