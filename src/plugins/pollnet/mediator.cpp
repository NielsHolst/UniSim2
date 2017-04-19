#include <base/publish.h>
#include "mediator.h"

using namespace base;

namespace pollnet {

PUBLISH(Mediator)

Mediator::Mediator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(B0).equals(0.5).help("Half-saturation density for feeding on floral rewards.");
    Input(h).equals(1.2).help("Hill coefficient--dictates shape of functional response.");
    Input(connectivityFile).imports("network[connectivityFile]");
    Input(R).help("Biomass of floral rewards of plant population.");
    Input(V).help("Biomass of vegetative plant population.");
    Input(B).help("Biomass of pollinator population.");
    Input(x).help("Metabolic rate of pollinator.");
    Input(y).help("Max. consumption rate");

    Output(Vsum);
}

void Mediator::amend() {
    // Find plants and pollinators
    plants = findMany<Box>("*<Plant>");
    pollinators = findMany<Box>("*<Pollinator>");
    numPlants = plants.size();
    numPollinators = pollinators.size();

    // Allocate vectors and matrices
    R.resize(numPlants);
    V.resize(numPlants);
    x.resize(numPollinators);
    y.resize(numPollinators);
    B.resize(numPollinators);
    CR.resize(numPollinators, numPlants);
    Gain_Pollinator.resize(numPollinators);
    Loss_Plant.resize(numPlants);

    // Create and connect outports ports
    setup_Consumption_Ports();
}

void Mediator::setup_Consumption_Ports() {
    // animal i, gains from eating
    for (int i = 0; i < numPollinators; ++i) {
        // Name Ports for Gain from Eating
        QString runningNumber = QString::number(i),
                gain_port_name = "gain"+runningNumber;
        NamedOutput(gain_port_name, Gain_Pollinator[i]);
        QString gain_port_path = fullName() + "[" + gain_port_name + "]";
        pollinators[i]->port("CR")->imports(gain_port_path);
    }

    // plant j, loss from being eaten
    for (int j = 0; j < numPlants; ++j) {
        // Name Ports for Gain from Eating
        QString runningNumber = QString::number(j),
                loss_port_name = "loss"+runningNumber;
        NamedOutput(loss_port_name, Loss_Plant[j]);
        QString loss_port_path = fullName() + "[" + loss_port_name + "]";
        plants[j]->port("CR")->imports(loss_port_path);
    }

}

void Mediator::initialize() {
    // Read connectivity matrix
    preference.read(connectivityFile, Table::NoLabelling);
    if (preference.numRow() != numPollinators)
        ThrowException("Mismatch of row number and pollinator number").
                value(preference.numRow()).
                value2(numPollinators).
                context(this);
    if (preference.numCol() != numPlants)
        ThrowException("Mismatch of column number and plant number").
                value(preference.numCol()).
                value2(numPlants).
                context(this);
}

void Mediator::reset() {
    B0_h = pow(B0,h);
}

void Mediator::update() {
    // NB! Compute Loss_Plant and Gain_Pollinator
    for (int i = 0; i < numPollinators; ++i) {
        double Byx = B[i] * y[i] * x[i];
        for (int j = 0; j < numPlants; ++j) {
            double F = compute_F( i, j);
            CR(i,j) = F * Byx;
        }
    }
    Vsum = compute_Vsum();
}

double Mediator::compute_F(int i, int j) {
    if (preference(i,j) == 0) return 0.;

    double F_numerator = preference(i,j) * pow(R[j],h);
    double F_denominator =  B0_h + F_numerator;
    return (R[j] <= 0) ? 0 : F_numerator/F_denominator;
}

double Mediator::compute_Vsum() {
    double Vtemp = 0;
    for (int i = 0; i < numPlants; ++i) {
        Vtemp = Vtemp + V[i];
    }
    return Vtemp;
}


}
