/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_FACE_H
#define SHELTER_FACE_H

#include "shelter_base.h"

namespace vg {

class ShelterFace : public ShelterBase
{
public:
    ShelterFace(QString name, QObject *parent);
    void amend();
};
} //namespace


#endif
