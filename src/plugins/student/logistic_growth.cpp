#include <base/publish.h>
#include "logistic_growth.h"

using namespace base;

namespace student {

PUBLISH(LogisticGrowth)

LogisticGrowth::LogisticGrowth(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial).equals(10).help("Initial population density (per m2)");
    Input(r).equals(0.012).help("Population growth rate (per day)");
    Input(K).equals(500).help("Carrying capacity (per m2)");
    Output(density);
}

void LogisticGrowth::reset() {
    density = initial;
}

void LogisticGrowth::update() {
    density += density*r*(K-density)/K;
}


}
