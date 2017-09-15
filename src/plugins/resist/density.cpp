#include <base/publish.h>
#include "check_vector.h"
#include "density.h"

using namespace base;

namespace resist {

PUBLISH(Density)

Density::Density(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(immigration).help("Number of immigrants of this genotype (ss,sr,rr)");
    Input(genotypeFrequency).help("Genotype frequency (ss,sr,rr");
    Input(reproduction).help("Reproduction by genotype (ss,sr,rr");
    Input(survival).help("Survival of genotype (ss,sr,rr");
    Input(survivalSS).help("Survival of SS genotype");
    Input(survivalSR).help("Survival of SR genotype");
    Input(survivalRR).help("Survival of RR genotype");
    Input(step).imports("/*[step]");
    Output(value).help("Density of this genotype");
}

void Density::reset() {
    value = immigration;
}

void Density::update() {
    if (step<2) return;
    CHECK_VECTOR_3(genotypeFrequency);
    CHECK_VECTOR_3(reproduction);
    CHECK_VECTOR_3(survival);

    const double
        &Rss(reproduction.at(0)),
        &Rsr(reproduction.at(1)),
        &Rrr(reproduction.at(2)),

        &Sss(survival.at(0)),
        &Ssr(survival.at(1)),
        &Srr(survival.at(2));

    Fss = Sss*Rss;
    Fsr = Ssr*Rsr;
    Frr = Srr*Rrr;

    if (objectName() == "ss")
        updateSS();
    else if (objectName() == "sr")
        updateSR();
    else if (objectName() == "rr")
        updateRR();
    else
        ThrowException("Object name ss, sr or rr expected").value(objectName());

    value += immigration;
}

void Density::updateSS() {
    const double
        &Pss(genotypeFrequency.at(0)),
        &Psr(genotypeFrequency.at(1));
    value =
        Fss*Pss +
        Fss*Psr/2 +
        Fsr*Pss/2 +
        Fsr*Psr/4;
}

void Density::updateSR() {
    const double
        &Pss(genotypeFrequency.at(0)),
        &Psr(genotypeFrequency.at(1)),
        &Prr(genotypeFrequency.at(2));
    value =
        Fss*Prr   +
        Frr*Pss   +
        Fss*Psr/2 +
        Frr*Psr/2 +
        Fsr*Pss/2 +
        Fsr*Psr/2 +
        Fsr*Prr/2;
}

void Density::updateRR() {
    const double
        &Psr(genotypeFrequency.at(1)),
        &Prr(genotypeFrequency.at(2));
    value =
        Frr*Prr   +
        Frr*Psr/2 +
        Fsr*Prr/2 +
        Fsr*Psr/4;
}

}
