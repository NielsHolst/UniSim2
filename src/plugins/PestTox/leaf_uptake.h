/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_LEAFUPTAKE_H
#define PESTTOX_LEAFUPTAKE_H

#include <base/box.h>

namespace PestTox {

class LeafUptake : public base::Box
{
public:
    LeafUptake(QString name, QObject *parent);
    void update();

private:
    // Input
    int leafType;
    double a, b, Tair, MV;

    // Output
    double V, Ed, log_kl, kl;
};

} //namespace


#endif
