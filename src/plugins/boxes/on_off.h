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
    bool onAtReset;
    // Output
    double value;
    bool isOn;
    // Methods
    void setSwitch();
    void setValue();
};

}

#endif
