#include <base/publish.h>
#include "Structural_dryweight.h"

using namespace base;

namespace saccharina {

PUBLISH(Structuraldryweight)

Structuraldryweight::Structuraldryweight(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the structural dryweight of the algae from the Area produced (g)");
    Input(kA).equals(0.6).help("Structural dry weight per unit area (g dm^-2");
    Input(A).equals(6).help("Area of the algae (dm)");
    Output(Ws).help("Structural dryweight of the algae (g)");
}

void Structuraldryweight::reset() {
   update();
}

void Structuraldryweight::update() {
   Ws = kA * A;
}


}
