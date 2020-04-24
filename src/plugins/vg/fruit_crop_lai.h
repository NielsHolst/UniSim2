/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FRUIT_CROP_LAI_H
#define FRUIT_CROP_LAI_H

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
