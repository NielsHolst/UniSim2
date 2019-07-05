/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MUSSEL_THINNING_H
#define MUSSEL_THINNING_H

#include <base/box.h>

namespace MusselBed {

class MusselThinning : public base::Box
{
public:
    MusselThinning(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double a, b, biomass, abundance, size, supply;

    // Outputs
    double maxAbundance, lossBiomass, lossAbundance;
};

} //namespace


#endif
