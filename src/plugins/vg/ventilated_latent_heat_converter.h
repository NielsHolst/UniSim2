/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_VENTILATED_LATENT_HEAT_CONVERTER_H
#define VG_VENTILATED_LATENT_HEAT_CONVERTER_H

#include "vapour_flux_base.h"

namespace vg {

class VentilatedLatentHeatConverter : public VapourFluxBase
{
public:
    VentilatedLatentHeatConverter(QString name, QObject *parent);
    void update();
private:
    double indoorsAh, groundArea, volumeFlowRate;
    double dewRate();
    double convertedAh();
};
} //namespace


#endif
