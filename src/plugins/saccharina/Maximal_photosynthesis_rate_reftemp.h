#ifndef MAXIMAL_PHOTOSYNTHETIC_RATE_REFTEMP_H
#define MAXIMAL_PHOTOSYNTHETIC_RATE_REFTEMP_H
#include <base/box.h>

namespace saccharina {

class Maximalphotosynthesisratereftemp : public base::Box
{
public:
    Maximalphotosynthesisratereftemp(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double P1, TAP, TP1, T, TAPL, TPL, TAPH, TPH;
    // Outputs
    double maxphotosynthesisreftemp;
};

}

#endif
