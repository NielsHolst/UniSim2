/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
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

private:
    double density;
};
} //namespace


#endif
