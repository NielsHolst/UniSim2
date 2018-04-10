/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SETPOINTS_H
#define SETPOINTS_H

#include <base/box.h>

namespace vg {

class Setpoints : public base::Box
{
public:
    Setpoints(QString name, QObject *parent);
    void amend();
};
} //namespace


#endif