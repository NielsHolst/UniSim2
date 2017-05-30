#ifndef Predator_H
#define Predator_H
#include <base/box.h>

namespace pinacate {

class Predator : public base::Box
{
public:
    Predator(QString name, QObject *parent);\

    void reset();
    void update();
private:
    // Inputs
    double demand;
    double juvSheepMass;
    double juvSheepAttack;
    double adultSheepMass;
    double adultSheepAttack;
    double juvDeerMass;
    double juvDeerAttack;
    double adultDeerMass;
    double adultDeerAttack;

    // Outputs
    double juvSheepMort;
    double adultSheepMort;
    double juvDeerMort;
    double adultDeerMort;
    double Supply;
    double juvSheepMortMass;
    double adultSheepMortMass;
    double juvDeerMortMass;
    double adultDeerMortMass;
};

}

#endif
