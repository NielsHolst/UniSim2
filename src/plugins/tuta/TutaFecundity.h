#ifndef TUTAFECUNDITY_H
#define TUTAFECUNDITY_H
#include <base/box.h>

namespace tuta {

class TutaFecundity : public base::Box
{
public: 
    TutaFecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a0, a1, a2, a3, a4, T;
    // Outputs
    double fecundity;
};

} //namespace
#endif
