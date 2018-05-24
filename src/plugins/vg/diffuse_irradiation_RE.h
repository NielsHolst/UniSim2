/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DIFFUSE_IRRADIATION_RE_H
#define DIFFUSE_IRRADIATION_RE_H

#include "diffuse_irradiation_base.h"

namespace vg {

class DiffuseIrradiationRE : public DiffuseIrradiationBase
{
public:
    DiffuseIrradiationRE(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double temperature, rh, sinb;
    // Outputs
    int eq;
    double fraction, value;
};
} //namespace


#endif
