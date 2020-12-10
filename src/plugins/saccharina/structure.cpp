#include <base/publish.h>
#include "structure.h"

using namespace base;

namespace saccharina {

PUBLISH(Structure)

Structure::Structure(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps track of structural mass");
    Input(proportionC).equals(0.20).unit("[0;1]").help("Proportion of the structural mass that is carbon");
    Input(proportionN).equals(0.01).unit("[0;1]").help("Proportion of the structural mass that is nitrogen");
    Input(allocation).unit("g").help("Allocated increment in mass");
    Input(initMass).equals(0.00125).unit("g").help("Initial mass");
    Output(mass).unit("g").help("Structural mass");
}

void Structure::reset() {
    mass = initMass;
}

void Structure::update() {
    mass += allocation;
}

}
