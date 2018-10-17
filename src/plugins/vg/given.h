/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GIVEN_H
#define GIVEN_H

#include <base/box.h>

namespace base {
    class BoxBuilder;
}

namespace vg {

class Given : public base::Box
{
public:
    Given(QString name, QObject *parent);
    void amend();
private:
    void amendAirFlux(base::BoxBuilder &builder);
    void amendVapourFlux(base::BoxBuilder &builder);
    void amendEnergyFlux(base::BoxBuilder &builder);
};
} //namespace


#endif
