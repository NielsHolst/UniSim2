/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_INDOORS_LIGHT_H
#define VG_INDOORS_LIGHT_H

#include <base/box.h>

namespace vg {

class IndoorsLight : public base::Box
{
public:
    IndoorsLight(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double sunlightDiffuse, sunlightDirect, sunlightPropPar, sunlightPhotonCoef,
        growthLigthtsDirect, growthLigthtsPar, growthLigthtsPhotonIntensity;

    // Outputs
    double direct, diffuse, total,
           parDirect, parDiffuse, parTotal,
           photonIntensity;
};
} //namespace


#endif
