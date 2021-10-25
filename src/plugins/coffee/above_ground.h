/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ABOVE_GROUND_H
#define ABOVE_GROUND_H
#include <base/box.h>

namespace coffee {

class AboveGround : public base::Box
{
public: 
    AboveGround(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double DAYL, solarRadiation, KEXT, LAI, KEXTT, LAIT;
    // Outputs
    double PARshade, PARav, PARint;
};

} //namespace
#endif
