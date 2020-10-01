#ifndef PMAX_H
#define PMAX_H
#include <base/box.h>

namespace saccharina {

class Pmax : public base::Box
{
public:
    Pmax(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double P1, TAP, TP1, T, TAPL, TPL, TAPH, TPH;
    // Outputs
    double value;
};

}

#endif
