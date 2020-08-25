/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_TRANSFER_LAYER_PARAMETERS_H
#define HEAT_TRANSFER_LAYER_PARAMETERS_H
#include <base/box.h>

namespace vg {

class HeatTransferLayerParameters : public base::Box
{
    friend class EnergyBudget;
public:
    HeatTransferLayerParameters(QString name, QObject *parent);
    void updateAbsorptivities();
protected:
    // Inputs
    double
        swReflectivityTop, swReflectivityBottom,
        swTransmissivityTop, swTransmissivityBottom,
        lwReflectivityTop, lwReflectivityBottom,
        lwTransmissivityTop, lwTransmissivityBottom,
        Utop, Ubottom,
        area, heatCapacity;
    // Outputs
    double
        swAbsorptivityTop, swAbsorptivityBottom,
        lwAbsorptivityTop, lwAbsorptivityBottom;
private:
    // Methods
    void checkRange(double x, QString name) const;
    void checkNotZero(double x, QString name) const;
};

} //namespace


#endif
