#include <base/publish.h>
#include "yield.h"

using namespace base;

namespace aphid {

PUBLISH(Yield)

Yield::Yield(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimates yield loss due to S. avenae");
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");;
    Input(aphidIndex).help("Aphid density index of Entwistle-Dixon-Wratten ");
    Output(yield).help("Yield relative to uninfested crop").unit("[0;1]");
    Output(loss).help("Proportional yield loss due to aphids").unit("[0;1]");;
    Output(lossRate).help("Loss rate").unit("d-1");
}

void Yield::reset() {
    yield = 1.;
    loss = lossRate = 0.;
}

void Yield::update() {
    const double &GS(cropGrowthStage); // create a shorthand name
    double Ei;
    if (aphidIndex > 0) {
        if (GS<53)
            Ei = 0.075;
        else if (GS<69)
            Ei = 0.205;
        else if (GS<71)
            Ei = 0.075;
        else if (GS<73)
            Ei = 0.056;
        else if (GS<77)
            Ei = 0.037;
        else
            Ei = 0.012;
        // daily yield loss due to aphids
        lossRate = aphidIndex*Ei/100.;
        // accumulated yield
        yield *= (1. - lossRate);
        // accumulated loss
        loss = 1. - yield;
    }
}

}
