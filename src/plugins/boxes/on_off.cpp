#include <base/exception.h>
#include <base/publish.h>
#include "on_off.h"

using namespace base;

namespace boxes {

PUBLISH(OnOff)

OnOff::OnOff(QString name, QObject *parent)
    : Box(name, parent)
{
    help("swithes on inside [xOn,xOff] interval");
    Input(x).help("x-value at which to calculate curve value");
    Input(xOn).equals(0).help("Lower threshold");
    Input(xOff).equals(1).help("Upper threshold");
    Input(valueOn).equals(1).help("Value inside [xOn,xOff] interval");
    Input(valueOff).equals(0).help("Value outside [xOn,xOff] interval");;
    Input(onAtReset).equals(false).help("Switch on at reset?");
    Output(value).help("Current on or off value");
    Output(isOn).help("Is switch on?");
}


void OnOff::reset() {
    isOn = onAtReset;
    setValue();
}

void OnOff::update() {
    setSwitch();
    setValue();
}

void OnOff::setSwitch() {
    isOn = (xOn <= x && x <= xOff);
}

void OnOff::setValue() {
    value = isOn ? valueOn : valueOff;
}


}
