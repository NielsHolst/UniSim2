#ifndef SEQUENCEW_H
#define SEQUENCEW_H
#include <base/box.h>

namespace boxes {

class Sequence : public base::Box
{
public:
    Sequence(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    int counter, counterMax;
    double min, max;
    QString interval;
    // Outputs
    double value;
};

}

#endif
