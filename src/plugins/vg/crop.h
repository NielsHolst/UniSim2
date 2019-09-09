/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CROP_H
#define CROP_H

#include <base/box.h>

namespace vg {

class Crop : public base::Box
{
public:
    Crop(QString name, QObject *parent);
    void amend();

private:
    double density;
    void amendLayer(QString level);
};
} //namespace


#endif
