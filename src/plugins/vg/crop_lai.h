/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_CROP_LAI_H
#define VG_CROP_LAI_H

#include <base/box.h>

namespace vg {

class CropLai : public base::Box
{
public:
    CropLai(QString name, QObject *parent);
    void reset();
protected:
    // Inputs
    double laiStartPerPlant, fractionPlantArea;
    // Outputs
    double value;
};
} //namespace


#endif
