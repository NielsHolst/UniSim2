/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CROP_LAI_H
#define CROP_LAI_H

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
