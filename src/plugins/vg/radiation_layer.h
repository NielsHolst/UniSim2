/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RADIATION_LAYER_H
#define RADIATION_LAYER_H
#include <base/box.h>

namespace vg {

class RadiationLayer : public base::Box
{
friend class RadiationLayers;
public:
    RadiationLayer(QString name, QObject *parent);
    void updateAbsorptivities();
protected:
    // Inputs
    double
        swReflectivityTop, swReflectivityBottom,
        swTransmissivityTop, swTransmissivityBottom,
        lwReflectivityTop, lwReflectivityBottom,
        lwTransmissivityTop, lwTransmissivityBottom,
        parFluxDown, parFluxUp,
        swFluxDown, swFluxUp,
        lwFluxDown, lwFluxUp,
        Utop, Ubottom,
        area, specificHeatCapacity, convectiveInflux, conductiveInflux ;
    // Outputs
    double
        swAbsorptivityTop, swAbsorptivityBottom,
        lwAbsorptivityTop, lwAbsorptivityBottom,
        parAbsorbedFromAbove, parAbsorbedFromBelow, parAbsorbed,
        swAbsorbedFromAbove, swAbsorbedFromBelow, swAbsorbed,
        lwAbsorbedFromAbove, lwAbsorbedFromBelow, lwAbsorbed,
        absorbed, temperature;
    // Methods
    void checkRange(double x, QString name) const;
    void updateLwFluxes();
    void updateTemperature(double timeStep);
};

} //namespace


#endif
