#include <base/publish.h>
#include "Maximal_photosynthesis_rate_reftemp.h"

using namespace base;

namespace saccharina {

PUBLISH(Maximalphotosynthesisratereftemp)

Maximalphotosynthesisratereftemp::Maximalphotosynthesisratereftemp(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the maximal photosynthesis at a reference temperature (Eq 13)");
    Input(P1).equals(0.00122).help("Maximal photosynthetic rate at T = TP1 in Kelvin");
    Input(TAP).equals(1694.4).help("Arrhenius temperature for photosynthesis");
    Input(TP1).equals(285).help("Reference temperature");
    Input(T).equals(285).help("Temperature");
    Input(TAPL).equals(27774).help("Arrhenius temperature for photosynthesis at low end of range");
    Input(TPL).equals(271).help("Low extreme temperature");
    Input(TAPH).equals(25924).help("Arrhenius temperature for photosynthesis at high end of range");
    Input(TPH).equals(296).help("High extreme temperature");
    Output(maxphotosynthesisreftemp).help("Maximal photosynthesis at a reference temperature");
}

void Maximalphotosynthesisratereftemp::reset() {
   update();
}

void Maximalphotosynthesisratereftemp::update() {
    maxphotosynthesisreftemp = (P1 * (exp((TAP/TP1)-(TAP/(T+273.15)))))/(1 + exp((TAPL/(T+273.15)-(TAPL/TPL)) + exp((TAPH/TPH)-(TAPH/(T+273.15)))));
}
}
