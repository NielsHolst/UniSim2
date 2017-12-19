#include <base/exception.h>
#include <base/publish.h>
#include "NesidiocorisFecundity.h"
using namespace base;

namespace tuta {

PUBLISH(NesidiocorisFecundity)

NesidiocorisFecundity::NesidiocorisFecundity(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes linear day-degrees");
    Input(a0).equals(0).help("a constant parameter");
    Input(a1).equals(0).help("a constant parameter");
    Input(a2).equals(0).help("a constant parameter");
    Input(a3).equals(0).help("a constant parameter");
    Input(a4).equals(0).help("a constant parameterd");
    Input(T).imports("weather[Tavg]");
    Output(fecundity).help("Value of Nesidiocoris fecundity");
}

void NesidiocorisFecundity::reset() {
    if (T >= 50) {
        QString msg{"T is very high"};
        ThrowException(msg).context(this);
    }
    update();
}

void NesidiocorisFecundity::update() {
    if (T > 40 || T < 15)
        fecundity = 0.0001;

    else
        fecundity = a0 * (pow(T,4)) + a1 * (pow(T,3)) + a2 * (pow(T,2)) + a3 * T + a4;

    if (fecundity <= 0)
        fecundity = 0.0001;
}

} //namespace

