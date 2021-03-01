/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ON_OFF_BASE_H
#define ON_OFF_BASE_H
#include <base/box.h>

namespace boxes {

template <class T> class OnOffBase : public base::Box
{
public:
    OnOffBase(QString name, QObject *parent) : Box(name, parent) {
        help("swithes on inside [xOn,xOff] interval");
        Input(x).help("x-value at which to calculate value");
        Input(xOn).help("Lower threshold");
        Input(xOff).help("Upper threshold");
        Input(valueOn).equals(1).help("Value inside [xOn,xOff] interval");
        Input(valueOff).equals(0).help("Value outside [xOn,xOff] interval");;
        Input(isSticky).equals(false).help("Should remain on when switched on?");
        Input(onAtReset).equals(false).help("Switch on at reset?");
        Output(value).help("Current on or off value");
        Output(isOn).help("Is switch on?");
    }
    void reset() {
        isOn = onAtReset;
        setValue();
    }
    void update() {
        if (!(isOn && isSticky)) {
            setSwitch();
            setValue();
        }
    }
protected:
    // Input
    T x, xOn, xOff;
    double valueOn, valueOff;
    bool onAtReset, isSticky;
    // Output
    double value;
    bool isOn;
    // Methods
private:
    virtual void setSwitch() = 0;
    virtual void setValue() = 0;
};

}

#endif
