/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_LEAF_LIGHT_RESPONSE_H
#define VG_LEAF_LIGHT_RESPONSE_H


#include <base/box.h>

namespace vg {

class LeafLightResponse : public base::Box
{
public:
    LeafLightResponse(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double rsCO2, rbCO2, Tleaf, co2Air,
           rhoChl, theta, frParAbs, concEnzyme;
    // Outputs
    double rtCO2, Pnmax, Pgmax, LUE, Rd;
    // Data
    double x25, TleafK, T25, rhoCo2T,
        VCmax, Jmax, KM, gamma;
    struct MichaelisMenten {
        double KC, KO;
        void update(double x25);
    };
    MichaelisMenten mm;
    // Methods
    double darkRespirationRate();
    double maxCarboxylationRate();
    double maxPhotosyntheticCapacity();
    double potentialLightUseEfficiency();
    double RubiscoCarboxylation();
    double co2CompensationConcentration();
    double maxNetAssimilation();
};
} //namespace


#endif
