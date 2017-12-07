#include <base/publish.h>
#include "check_vector.h"
#include "density.h"

using namespace base;

namespace resist {

PUBLISH(Density)

Density::Density(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(immigration).help("Immigrants vector (ss,sr,rr)");
    Input(reproduction).help("Reproduction vector (ss,sr,rr)");
    Input(survival).help("Survival vector by genotype (ss,sr,rr)");
    Output(ss).help("Density of ss genotype");
    Output(sr).help("Density of sr genotype");
    Output(rr).help("Density of rr genotype");
    Output(values).help("Density vector (ss, sr, rr)");
    values.resize(3);
}

void Density::reset() {
    ss = immigration[0];
    sr = immigration[1];
    rr = immigration[2];
}

void Density::update() {
    CHECK_VECTOR_3(immigration);
    CHECK_VECTOR_3(reproduction);
    CHECK_VECTOR_3(survival);
    values[0] = ss = immigration[0] + survival[0]*reproduction[0];
    values[1] = sr = immigration[1] + survival[1]*reproduction[1];
    values[2] = rr = immigration[2] + survival[2]*reproduction[2];
}

}
