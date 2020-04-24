/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VAPOUR_FLUX_COVER_CONDENSATION_H
#define VAPOUR_FLUX_COVER_CONDENSATION_H

#include "vapour_flux_base.h"

namespace vg {

class VapourFluxCondensation : public VapourFluxBase
{
public:
    VapourFluxCondensation(QString name, QObject *parent);
    void update();
private:
    double surfaceTemperature, indoorsTemperature, indoorsAh, surfaceAreaPerGroundArea;
};
} //namespace


#endif
