#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "functional_response.h"
using namespace base;

namespace boxes {

PUBLISH(FunctionalResponse)

FunctionalResponse::FunctionalResponse(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes a Gutierrez-Baumgärtner functional response");
    Input(attacker).help("Density of attacker (predator, parasitoid)");
    Input(resource).help("Density of resource (prey, host)");
    Input(demand).help("Demand of attacker");
    Input(attackRate).help("Attack rate per attacker per time unit");
    Input(timeStep).equals(1).help("Time step in units of attackRate"),
    Output(searchRate).help("Search rate [0;1]");
    Output(resourceAcquired).help("The amount of resource acquired (killed, parasitized)");
    Output(resourceMortality).help("Mortality on resource [0;1]");
}

void FunctionalResponse::update() {
    searchRate = 1. - exp(-attackRate*attacker*timeStep);
    resourceAcquired = (demand == 0.)? 0. : demand*(1. - exp(-searchRate*resource/demand));
    resourceMortality = (resource == 0.) ? 0. : resourceAcquired/resource;
    // Rounding off error
    if (resourceMortality > 1.)
        resourceMortality = 1.;
}

} //namespace

