#ifndef TUTA_H
#define TUTA_H
#include <base/box.h>

namespace tuta {

class Tuta : public base::Box
{
public: 
    Tuta(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a0, a1, a2, a3, T0, T, timeStepDays;
    bool resetTotal, isTicking;
    // Outputs
    double step, total;
};

} //namespace
#endif
