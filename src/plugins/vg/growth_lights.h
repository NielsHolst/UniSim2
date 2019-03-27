/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_GROWTH_LIGHTS_H
#define VG_GROWTH_LIGHTS_H

#include <QVector>
#include "growth_light_base.h"

namespace vg {

class GrowthLights : public GrowthLightBase
{
public:
    GrowthLights(QString name, QObject *parent);
};
} //namespace


#endif
