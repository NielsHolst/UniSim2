#include <base/publish.h>
#include "Wetweight.h"

using namespace base;

namespace saccharina {

PUBLISH(Wetweight)

Wetweight::Wetweight(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the wetweight of the algae from the Area produced (g)");
    Input(kA).equals(0.6).help("Structural dry weight per unit area (g dm^-2");
    Input(kdw).equals(0.0785).help("Dry weight to wet weight ratio of structural mass");
    Input(kN).equals(2.72).help("Mass of nitrogen reserves per gram nitrogen (g(g N)^-1)");
    Input(N).equals(0.01).help("Nitrogen reserve (g N (gsw)^-1))");
    Input(Nmin).equals(0.01).help("Minimal nitrogen reserve (g N (gsw)^-1)");
    Input(kC).equals(2.1213).help("Mass of carbon reserves per gram carbon (g(g C)^-1)");
    Input(C).equals(0.01).help("Carbon reserves (g C (gsw)^-1)");
    Input(Cmin).equals(0.01).help("Minimum carbon reserves (g N (gsw)^-1)");
    Input(A).equals(6).help("Area of the algae (dm)");
    Output(Ww).help("Wetweight of the algae (g)");
}

void Wetweight::reset() {
   update();
}

void Wetweight::update() {
   Ww = kA * (pow(kdw,-1) + kN * (N-Nmin) + Nmin + kC * (C-Cmin) + Cmin)*A;
}


}
