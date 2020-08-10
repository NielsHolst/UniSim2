/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_TRANSFER_VOLUME_H
#define HEAT_TRANSFER_VOLUME_H
#include <base/box.h>


namespace vg {

class HeatTransferVolume : public base::Box
{
    friend class EnergyBudget;
public:
    HeatTransferVolume(QString name, QObject *parent);
    void reset() final;
    void update() final;
protected:
    // Inputs
    double initTemperature, convectiveInflux,
        outdoorsInfluxVolume, outdoorsTemperature,
        neighbourInfluxVolume, neighbourTemperature,
        timeStep, groundArea, volume;
    // Outputs
    double temperature, advectiveEnergyFlux;
private:
    // Methods
    void updateByAdvection(double influxVolume, double influxTemperature);
};

} //namespace
#endif
