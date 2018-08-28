/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_FLUX_VENTS_H
#define AIR_FLUX_VENTS_H

#include "air_flux_base.h"

namespace vg {

class Vent;

class AirFluxVents : public AirFluxBase
{
public:
    AirFluxVents(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Inputs
    QString pathToVents;
    double ventOpening,
        roofPitch, greenhouseLength,
        indoorsVolumeTotal, indoorsTemperature,
        outdoorsTemperature, windSpeed;
    // Output
    double ventLength, ventWidth, ventTransmissivity;
    // Data
    QVector<Vent*> vents;
    QVector<double> areas;
    double sumAreas;
    // Methods
    void updateTransmissivity();
};
} //namespace


#endif
