/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_INDOORS_WIND_SPEED_H
#define VG_INDOORS_WIND_SPEED_H

#include <base/box.h>

namespace vg {

class IndoorsWindSpeed : public base::Box
{
public:
    IndoorsWindSpeed(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double constructionWidth, ventilation;

    // Variables
    double value;
};
} //namespace


#endif
