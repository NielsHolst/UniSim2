/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_SPACE_SCREENED_H
#define AIR_SPACE_SCREENED_H
#include "air_space_base.h"


namespace vg {

class AirSpaceScreened : public AirSpaceBase
{
public: 
    AirSpaceScreened(QString name, QObject *parent=0);
    void initialize();
    void updateVolume();
protected:
    // Inputs
    double area, depth, roofHeight;
};

} //namespace
#endif
