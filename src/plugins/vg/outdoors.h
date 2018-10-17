/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_OUTDOORS_H
#define VG_OUTDOORS_H

#include <base/box.h>

namespace vg {

class Outdoors : public base::Box
{
public:
    Outdoors(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Parameters
    double temperature, skyTemperature, windSpeed, rh,
    radiation, diffuseRadiation, sunlightPhotonCoef,
    co2, soilTemperature;

    // Variables
    double directRadiation, propDirectRadiation, ah, sh;

};
} //namespace


#endif
