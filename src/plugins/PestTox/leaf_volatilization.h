/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_LEAFVOLATILIZATION_H
#define PESTTOX_LEAFVOLATILIZATION_H

#include <base/box.h>

namespace PestTox {

class LeafVolatilization : public base::Box
{

public:
    LeafVolatilization(QString name, QObject *parent);
    void update();

private:
    // Input
    double VP, Ea, Doseappl, Tair;

    // Output
    double ln_ER, ER, fv, fr, kTr, kl;


};

} //namespace


#endif
