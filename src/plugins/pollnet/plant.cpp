#include <base/publish.h>
#include "plant.h"

using namespace base;

namespace pollnet {

PUBLISH(Plant)

Plant::Plant(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial_R).imports("random/plant/initial_R[value]").help("Initial biomass of floral rewards.");
    Input(initial_V).imports("random/plant/initial_V[value]").help("Initial biomass of vegetative portion of plant population.");
    Input(m).imports("random/plant/m[value]").help("body mass of plant");

    Input(s).equals(2).help("Self-limitation rate of floral rewards production.");
    Input(beta).equals(1).help("Producton rate of floral rewards.");
    Input(kappa).imports("random/plant[kappa]").help("Cost of producing floral rewards in terms of vegetative biomass.");
    Input(K).equals(5).help("Plant community-wide carrying capcity.");

    Input(Vsum).help("Total plant biomass");
    Input(CR).help("Consumption rate; computed by mediator");

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
    double pollinator_consumption = CR;

    double benefit_accrual = pollinator_consumption/(1+pollinator_consumption);
    return benefit_accrual;
}

double Plant::compute_K_effect() {
    double K_effect = 1 - (Vsum/K);
    return K_effect;
}

double Plant::compute_dRdt() {

    return compute_floral_growth() - CR;
}

double Plant::compute_dVdt() {
    double K_effect = compute_K_effect();
    double benefit_accrual = compute_benefit_accrual();
    double floral_growth = compute_floral_growth();

    double dVdt = K_effect*r*V*benefit_accrual - x*V - kappa*floral_growth;

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
