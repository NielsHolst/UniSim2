/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CROP_RADIATION_H
#define CROP_RADIATION_H

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
