/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_FACE_AREA_H
#define SHELTER_FACE_AREA_H

#include <base/box.h>

namespace vg {

class ShelterFaceArea : public base::Box
{
public:
    ShelterFaceArea(QString name, QObject *parent);
    void reset();

private:
    // Inputs
    double roofArea, sideWallsArea, endWallsArea, gablesArea, groundArea;
    // Outputs
    double value, relativeArea, areaPerGround;

};
} //namespace


#endif
