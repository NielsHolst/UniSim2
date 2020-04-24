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
public:
    RadiationLayer(QString name, QObject *parent);
    void reset();
    void update();
    static double swTransmissivityTotal(QVector<const RadiationLayer*> layers);
    static double lwTransmissivityTotal(QVector<const RadiationLayer*> layers);
    static QVector<double> lwEmissivityTotal(QVector<const RadiationLayer*> layers);
private:
    // Inputs
    double
        swReflectivityOut, swReflectivityIn, swTransmissivity,
        lwReflectivityOut, lwReflectivityIn, lwTransmissivity;
    // Outputs
    double
        swAbsorptanceOut, swAbsorptanceIn,
        lwEmissivityOut, lwEmissivityIn;
    // Methods
    void checkRange(double x, QString name) const;
};

} //namespace


#endif
