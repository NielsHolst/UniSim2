/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_HEAT_PIPES_H
#define ACTUATOR_HEAT_PIPES_H

#include <base/box.h>

namespace vg {

class ActuatorHeatPipes : public base::Box
{
public:
    ActuatorHeatPipes(QString name, QObject *parent);
    void reset();
    void update();
    double getTemperature() const;
    void setTemperature(double value);
private:
    // Inputs
    double temperature, propConvection;
    QVector<double> minTemperatures, maxTemperatures, energyFluxes;
    // Outputs
    double minTemperature, maxTemperature, energyFluxConvection, energyFluxRadiation, energyFluxTotal;
};
} //namespace


#endif