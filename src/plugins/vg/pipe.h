/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_PIPE_H
#define VG_PIPE_H

#include <base/box.h>

namespace vg {

class Pipe : public base::Box
{
public:
    Pipe(QString name, QObject *parent);
private:
    // Inputs
    double density, diameter, flowRate, minTemperature, maxTemperature, a, b, emissivity;
};

} //namespace


#endif
