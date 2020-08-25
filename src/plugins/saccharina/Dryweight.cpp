#include <base/publish.h>
#include "Dryweight.h"

using namespace base;

namespace saccharina {

PUBLISH(Dryweight)

Dryweight::Dryweight(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the dryweight of the algae from the Area produced (g)");
    Input(kA).equals(0.6).help("Structural dry weight per unit area (g dm^-2");
    Input(kN).equals(2.72).help("Mass of nitrogen reserves per gram nitrogen (g(g N)^-1)");
    Input(N).equals(0.01).help("Nitrogen reserve (g N (gsw)^-1))");
    Input(Nmin).equals(0.01).help("Minimal nitrogen reserve (g N (gsw)^-1)");
    Input(kC).equals(2.1213).help("Mass of carbon reserves per gram carbon (g(g C)^-1)");
    Input(C).equals(0.01).help("Carbon reserves (g C (gsw)^-1)");
    Input(Cmin).equals(0.01).help("Minimum carbon reserves (g N (gsw)^-1)");
    Input(A).equals(6).help("Area of the algae (dm)");
    Output(Wd).help("Dryweight of the algae (g)");
}

void Dryweight::reset() {
   update();
}

void Dryweight::update() {
   Wd = kA * (1 + kN * (N-Nmin) + Nmin + kC * (C-Cmin) + Cmin)*A;
}


}
