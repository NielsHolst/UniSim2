/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_ENERGY_FLUX_HEATING
#define VG_ENERGY_FLUX_HEATING

#include <QVector>
#include "energy_flux_base.h"

namespace vg {

class EnergyFluxHeating : public EnergyFluxBase
{
public:
    EnergyFluxHeating(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    QVector<double> density, diameter, flowRate, a, b;
    double inflowTemperature, indoorsTemperature, groundArea;
    // Outputs
    QVector<double> pipeLength, pipeVolume, transitTime,
        outflowTemperature, temperatureDrop, energyFlux;
    // Data
    int n;
    // Methods
    void updatePipe(int i);
};

} //namespace

#endif
