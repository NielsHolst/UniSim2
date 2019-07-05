/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MUSSEL_H
#define MUSSEL_H

#include <base/box.h>

namespace MusselBed {

class Mussel : public base::Box
{
public:
    Mussel(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double initBiomass, initAbundance, lossBiomass, lossAbundance, growth;
    int day;

    // Outputs
    double abundance, biomass, size, LD;
    int LD50;
};

} //namespace


#endif
