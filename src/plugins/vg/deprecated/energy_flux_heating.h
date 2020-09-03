/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_FLUX_HEATING
#define ENERGY_FLUX_HEATING

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
    QVector<double> energyFluxes;
};

} //namespace

#endif
