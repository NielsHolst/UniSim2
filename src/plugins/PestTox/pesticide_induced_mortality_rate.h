/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_PESTICIDEINDUCEDMORTALITYRATE_H
#define PESTTOX_PESTICIDEINDUCEDMORTALITYRATE_H

#include <base/box.h>

namespace PestTox {

class pesticideinducedmortalityrate : public base::Box
{
public:
    pesticideinducedmortalityrate(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double LC50, concentration, slope, ar;

    // Output
    double C, mortalityRate, survivalRate;
};

} //namespace


#endif
