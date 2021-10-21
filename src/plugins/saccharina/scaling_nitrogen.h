/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCALING_NITROGEN_H
#define SCALING_NITROGEN_H
#include <base/box.h>

namespace saccharina {

class ScalingNitrogen : public base::Box
{
public:
    ScalingNitrogen(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double minValue, concN, optNConc;
    // Outputs
    double value;
};

}

#endif
