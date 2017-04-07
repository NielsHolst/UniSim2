#include <base/publish.h>
#include "mediator.h"

using namespace base;

namespace student {

PUBLISH(Mediator)

Mediator::Mediator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(preference).equals(1).help("Matrix (?) of preferences for plants.");
    Input(B0).equals(0.5).help("Half-saturation density for feeding on floral rewards.");
    Input(h).equals(1.2).help("Hill coefficient--dictates shape of functional response.");

    Input(R).help("Biomass of floral rewards of plant population.");

    Input(x).help("Metabolic rate of pollinator.");
    Input(y).help("Max. consumption rate");
    Input(B).help("Biomass of pollinator population.");

    Output(cr);
}

void Mediator::reset() {
}

void Mediator::update() {
    double F = compute_F();
    cr = F*B*y*x;
}

double Mediator::compute_F() {
    double F_numerator = preference * pow(R,h);
    double F_denominator = pow(B0,h) + preference * pow(R,h);
    return (R <= 0) ? 0 : F_numerator/F_denominator;
}


}
