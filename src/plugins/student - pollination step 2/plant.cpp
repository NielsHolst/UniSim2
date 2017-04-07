#include <base/publish.h>
#include "plant.h"

using namespace base;

namespace student {

PUBLISH(Plant)

Plant::Plant(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial_R).equals(1).help("Initial biomass of floral rewards.");
    Input(initial_V).equals(5).help("Initial biomass of vegetative portion of plant population.");
    Input(r).help("Intrinsic growth rate of plant population.");
    Input(s).equals(2).help("Self-limitation rate of floral rewards production.");
    Input(beta).equals(1).help("Producton rate of floral rewards.");
    Input(kappa).equals(1).help("Cost of producing floral rewards in terms of vegetative biomass.");
    Input(K).equals(5).help("Plant community-wide carrying capcity.");
    Input(m).equals(100).help("body mass of plant");

    Input(Vsum).help("Total plant biomass");
    Input(cr).help("Consumption rate; computed by mediator");

    Output(V).help("Biomass of vegetative portion of plant population.");
    Output(R).help("Biomass of floral rewards of plant population.");
}

void Plant::reset() {
    // Allometry
    x = pow(m, 0.75);
    r = x/0.138;

    R = initial_R;
    V = initial_V;
}

double Plant::compute_floral_growth() {
    double floral_growth = beta*V - s*R;
    return floral_growth;
}


double Plant::compute_benefit_accrual() {
    double pollinator_consumption = cr;

    double benefit_accrual = pollinator_consumption/(1+pollinator_consumption);
    return benefit_accrual;
}

double Plant::compute_K_effect() {
    double K_effect = 1 - (Vsum/K);
    return K_effect;
}

double Plant::compute_dRdt() {
//    double floral_growth = compute_floral_growth();
//    double pollinator_consumption = cr;

//    double dRdt = floral_growth - pollinator_consumption;

//    return dRdt;
    return compute_floral_growth() - cr;
}

double Plant::compute_dVdt() {
    double K_effect = compute_K_effect();
    double benefit_accrual = compute_benefit_accrual();
    double floral_growth = compute_floral_growth();

    double dVdt = K_effect*r*V*benefit_accrual - x*B - kappa*floral_growth;

    return dVdt;
}

void Plant::update() {
    // compute_new_R, compute_new_V
    const double dt = 0.001;
    double dRdt = compute_dRdt();
    double dVdt = compute_dVdt();

    R = R + dRdt * dt;
    V = V + dVdt * dt;

    if(R <= 0) {
        R = 0;
    }
    if(V <= 0) {
        V = 0;
        R = 0;
    }
}

}
