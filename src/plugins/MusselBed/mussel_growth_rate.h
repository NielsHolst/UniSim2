/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MUSSEL_GROWTH_RATE_H
#define MUSSEL_GROWTH_RATE_H

#include <base/box.h>

namespace MusselBed {

class MusselGrowthRate : public base::Box
{
public:
    MusselGrowthRate(QString name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    // Inputs
    double a, b, carryingCapacity, size, biomass;

    // Outputs
    double value;

    // Links
    QVector<base::Box*> scales;
};

} //namespace


#endif
