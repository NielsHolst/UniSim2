/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COVER_H
#define COVER_H
#include <base/box.h>
#include "radiation_layer.h"

namespace vg {

class Cover : public RadiationLayer
{
public:
    Cover(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double
        UwindMinimum, UwindSlope, windSpeed, haze;
};
} //namespace


#endif
