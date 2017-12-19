#ifndef NESIDIOCORIS_H
#define NESIDIOCORIS_H
#include <base/box.h>

namespace tuta {

class Nesidiocoris : public base::Box
{
public: 
    Nesidiocoris (QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double P, m, n, Tu, T0, T, timeStepDays;
    bool resetTotal, isTicking;
    // Outputs
    double step, total;
};

} //namespace
#endif
