#include <base/publish.h>
#include "pollinator.h"

using namespace base;

namespace student {

PUBLISH(Pollinator)

Pollinator::Pollinator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial_B).equals(1).help("Initial biomass of pollinator population.");
    Input(R).help("Biomass of floral rewards of plant population.");

    Input(m).equals(1).help("Body mass of pollinator in __ units.");
    Input(is_vertebrate).equals(false).help("Boolean varable, TRUE if vertebrate.");


    Input(cr).help("Consumption rate; computed by mediator");

    Output(x).help("Metabolic rate of pollinator.");
    Output(y).help("Max. consumption rate");
    Output(B).help("Biomass of pollinator population.");

}

void Pollinator::reset() {
    // Allometry
    x = pow(m, 0.75);
    // double eij = 0.45;

    y = is_vertebrate ? 4 : 8;

    B = initial_B;
}


double Pollinator::compute_dBdt() {
//    double F = compute_F();
    return cr-x*B;
}

void Pollinator::update() {
    // compute_new_B
    const double dt = 0.001;
    double dBdt = compute_dBdt();
    B = B +  dBdt * dt;

    if(B <= 0) {
        B = 0;
    }
}

}
