/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STARFISH_TEMP_SCALE_H
#define STARFISH_TEMP_SCALE_H

#include <base/box.h>

namespace MusselBed {

class StarfishTempScale : public base::Box
{
public:
    StarfishTempScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double a, b, temperature;

    // Outputs
    double value;
};

} //namespace


#endif

