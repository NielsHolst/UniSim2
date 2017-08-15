#include <base/exception.h>
#include <base/publish.h>
#include "functional_response.h"
using namespace base;

namespace boxes {

PUBLISH(FunctionalResponse)

FunctionalResponse::FunctionalResponse(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes Gutierrez-Baumgärtner functional response");
    Input(resource).help("Density of resource");
    Input(demand).help("Demand of attacker");
    Input(apparency).help("Apparency of resource");
    Output(supply).help("Supply for attacker");
    Output(resourceMortality).help("Mortality on resource [0;1]");
}

void FunctionalResponse::update() {
    supply = (demand == 0.)
              ? 0.
              : demand*(1. - exp(-apparency*resource/demand));
    if (supply>resource)
        supply = resource;
    resourceMortality = (resource == 0.) ? 0. : supply/resource;
}

} //namespace

