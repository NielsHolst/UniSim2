/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include <base/box.h>

namespace vg {

class Accumulator : public base::Box
{
public:
    Accumulator(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double initial, change;
    // Outputs
    double value;
};
} //namespace


#endif
