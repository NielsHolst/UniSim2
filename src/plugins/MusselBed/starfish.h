/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STARFISH_H
#define STARFISH_H

#include <base/box.h>

namespace MusselBed {

class Starfish : public base::Box
{
public:
    Starfish(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double stinBiomass, stinAbundance, supply, respiration;

    // Outputs
    double stBiomass, stAbundance, stSize;
};

} //namespace


#endif

