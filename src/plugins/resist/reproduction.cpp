#include <base/publish.h>
#include "reproduction.h"

using namespace base;

namespace resist {

PUBLISH(Reproduction)

Reproduction::Reproduction(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(N).help("Population size of this genotype");
    Input(R).help("Net reproductive rate of this genotype (per capita per generation)");
    Input(survival).help("Survival of this genotype (per generation) [0;1]");
    Output(value).help("Reproduction of this genotype (per generation");
}

void Reproduction::update() {
    value = N*R*survival;
}

}
