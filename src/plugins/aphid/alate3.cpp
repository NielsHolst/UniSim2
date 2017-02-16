#include <base/publish.h>
#include "alate3.h"
#include <base/exception.h> // because I want to reject negative density to avoid my program to crash if it is the case

using namespace base;

namespace aphid {

PUBLISH(Alate3)

Alate3::Alate3(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (density);
    Input (percentage_senescence);
    Input (b1);
    Input (a1);
    Input (a2);
    Input (b2);
    Output(proportion);
    }

void Alate3::reset() {
    proportion=0;
}

void Alate3::update() {
    // In Watt and Dixon 1981: first alate produce when density is 0.6 aphids per tiller and maximum proportion
    // of alate is 0.9 and not 1. Will have to work more to find better parameters estimates then the one proposed here.
    // problem with the eqation in this paper: built one just to keep going.
    if (density < 0)
       ThrowException("Negative density").value(density).context(this);
    else if (density==0)
       proportion = 0;
    //if the crop is not senescent yet: a1=2.1972, b1=12
    else if (percentage_senescence < 1e-6)
       proportion = 1/(1+exp(-b1*(log(density)-a1)));
    // if the crop starts being senescent: a2= 2.1972, b2=1
    else
       proportion = 1/(1+exp(-b2*(log(density)-a2)));
}
}
