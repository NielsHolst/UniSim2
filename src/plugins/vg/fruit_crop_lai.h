/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_FRUIT_CROP_LAI_H
#define VG_FRUIT_CROP_LAI_H

#include "crop_lai.h"

namespace vg {

class FruitCropLai : public CropLai
{
public:
    FruitCropLai(QString name, QObject *parent);
    void reset();
    void update();

private:
    double cropDensity, tempSum,
        laiStart, maxLai;
    bool cropPresent, maxLaiReached;
};
} //namespace


#endif
