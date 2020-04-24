/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_H
#define SHELTER_H
#include "shelter_base.h"

namespace base {
    class Box;
    class BoxBuilder;
}

namespace vg {

class Shelter : public ShelterBase
{
public:
    Shelter(QString name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Inputs
    QVector<double> screenVolumes;
    // Outputs
    double volumeBehindScreens;
    // Methods
    void amendShelter(base::BoxBuilder &builder, QString shelterName);
};
} //namespace


#endif
