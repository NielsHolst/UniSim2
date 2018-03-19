/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_CROP_RADIATION_H
#define VG_CROP_RADIATION_H

#include <base/box.h>

namespace vg {

class CropRadiation : public base::Box
{
public:
    CropRadiation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kDiffuse, scattering, sinb,
        lightDiffuse, lightDirect,
        absorptivityTop, absorptivityMiddle, absorptivityBottom;

    // Outputs
    double
        kDirect, kDirectDirect,
        diffuseReflectivity, directReflectivity,
        reflectivity;
};
} //namespace


#endif
