/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DIFFUSE_IRRADIATION_BASE_H
#define DIFFUSE_IRRADIATION_BASE_H

#include <base/box.h>

namespace vg {

class DiffuseIrradiationBase : public base::Box
{
public:
    DiffuseIrradiationBase(QString name, QObject *parent);
    void updateClearness();

protected:
    // Inputs
    double angot, globalIrradiation;
    // Outputs
    double clearness;
};
} //namespace


#endif
