/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ON_OFF_H
#define ON_OFF_H
#include <base/box.h>

namespace boxes {

class OnOff : public base::Box
{
public:
    OnOff(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double x, xOn, xOff, valueOn, valueOff;
    bool onAtReset, isSticky;
    // Output
    double value;
    bool isOn;
    // Methods
    void setSwitch();
    void setValue();
};

}

#endif
