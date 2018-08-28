/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DAYLIGHT_LEVEL_H
#define DAYLIGHT_LEVEL_H

#include <base/box.h>

namespace vg {

class DaylightLevel : public base::Box
{
public:
    DaylightLevel(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double dawnThreshold, duskThreshold, outdoorsRadiation;
    QTime time;
    // Outputs
    bool isDay, isNight;
    int day, night;
};
} //namespace


#endif
