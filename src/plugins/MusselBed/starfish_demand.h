/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STARFISH_DEMAND_H
#define STARFISH_DEMAND_H

#include <base/box.h>

namespace MusselBed {

class StarfishDemand : public base::Box
{
public:
    StarfishDemand(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double stSize;
    double temperature;
    double stBiomass;

    // Outputs
    double netDemand;
    double Respiration;

};

} //namespace


#endif
