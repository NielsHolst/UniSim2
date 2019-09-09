/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_CROP_YIELD_H
#define VG_CROP_YIELD_H

#include <base/box.h>

namespace vg {

class CropYield : public base::Box
{
public:
    CropYield(QString name, QObject *parent);
    void reset();
    void update();
protected:
    // Inputs
    double produceMass, fractionPlantArea, fractionDryWeight;
    // Outputs
    double freshWeight, dryWeight;
};
} //namespace


#endif
