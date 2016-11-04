/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_PRIMARYDISTRIBUTIONSOIL_H
#define PESTTOX_PRIMARYDISTRIBUTIONSOIL_H

#include <base/box.h>

namespace PestTox {

class PrimaryDistributionSoil : public base::Box
{
public:
    PrimaryDistributionSoil(QString name, QObject *parent);
    void update();

private:
    // Input
    double Doseappl, Doserl, Dosedrift;

    // Output
    double Dosers;
};

} //namespace


#endif
