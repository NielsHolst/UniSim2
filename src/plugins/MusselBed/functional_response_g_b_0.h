/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef FUNCTIONAL_RESPONSE_G_B
#define FUNCTIONAL_RESPONSE_G_B

#include <base/box.h>

namespace MusselBed {

class FunctionalResponseGB : public base::Box
{
public:
    FunctionalResponseGB(QString name, QObject *parent);
    void update();

private:
    // inputs
    double apparency, resourceDensity, demand,
        egestionRatio, respiration,
        conversionCost;
    bool truncateSupply, costOnRespiration;


    // outputs
    double supply, sdRatio, totalDemand, totalSupply, egestion,
        attacksPerHost, numHostsAttacked, propHostsAttacked;

    // methods
    static double f(double demand, double resource);
    static double divBounded(double x, double y, double bound);
};

} //namespace


#endif
