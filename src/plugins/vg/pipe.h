/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_PIPE_H
#define VG_PIPE_H

#include <base/box.h>

namespace vg {

class Pipe : public base::Box
{
public:
    Pipe(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double length, diameter, minTemperature, maxTemperature, maxTemperatureIncreaseRate, emissivity,
        energyFluxTotal, indoorsTemperature, timeStep;
    // Outputs
    double temperature, energyFlux,
        nextTemperatureMin, nextTemperatureMax,
        nextEnergyFluxMin, nextEnergyFluxMax;

    // Data
    const double exponent{1.25};
    double slope;
    QVector<const double*> energyFluxFromPreceedingPipes;

    // Methods
    QList<base::Box*> seekPrecedingSiblings();
    double calcEnergyFlux(double temperatureDifference) const;
    double calcTemperatureDifference(double energyFluxTotal) const;
    void setNextTemperatureMin();
    double energyFluxFromPreceedingPipesSum();
};

} //namespace


#endif
