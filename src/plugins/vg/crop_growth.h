/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_CROP_GROWTH_H
#define VG_CROP_GROWTH_H

#include <base/box.h>

namespace vg {

class CropGrowth : public base::Box
{
public:
    CropGrowth(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double Tcrop, Pg, timeStepSecs,
        massRoot, massStem, massLeaf, massFruit,
        respRoot, respStem, respLeaf, respFruit,
        grossGrowthRate, netGrowthRate, maintenanceRespirationRate,
        netGrowthAllocation;
    // Methods
    double _maintenanceRespirationRate();
};
} //namespace


#endif
