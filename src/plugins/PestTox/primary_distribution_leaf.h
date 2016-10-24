/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_PRIMARYDISTRIBUTIONLEAF_H
#define PESTTOX_PRIMARYDISTRIBUTIONLEAF_H

#include <base/box.h>

namespace PestTox {

class primarydistributionleaf : public base::Box
{
public:
    primarydistributionleaf(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Doseappl, Dosedrift,
        stage1, stage2, stage3, stage4;

    // Output
    double Doserl, fi;

};

} //namespace


#endif
