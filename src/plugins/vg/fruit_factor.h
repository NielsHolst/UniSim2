/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_FRUIT_FACTOR_H
#define VG_FRUIT_FACTOR_H

#include <base/box.h>


namespace vg {

class FruitFactor : public base::Box
{
public:
    FruitFactor(QString name, QObject *parent);
    void reset();
    void update();
private:
    double maxValue, minDensity, plantDensity, value;
};
} //namespace


#endif
