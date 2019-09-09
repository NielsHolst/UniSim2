/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VENT_H
#define VENT_H

#include <base/box.h>

namespace vg {

class Vent : public base::Box
{
public:
    Vent(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    int number;
    double length, width, state,
        ventTransmissivity, screensTransmissivity;
    // Output
    double transmissivity;
};

} //namespace

#endif
