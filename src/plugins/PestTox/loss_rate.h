/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LOSS_RATE_H
#define LOSS_RATE_H

#include <base/box.h>

namespace PestTox {

class LossRate : public base::Box
{
public:
    LossRate(QString name, QObject *parent);
    void reset() final;
    void update() final;
private:
    // Output
    double instantaneous, finite;
    // Methods
    virtual double computeInstantaneous() = 0;
};

} //namespace


#endif
