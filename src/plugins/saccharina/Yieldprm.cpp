#include <base/publish.h>
#include "Yieldprm.h"

using namespace base;

namespace saccharina {

PUBLISH(Yieldprm)

Yieldprm::Yieldprm(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the yield pr meter of cultivation line (g)");
    Input(Ww).equals(1).help("Wet weight of each algae (kg))");
    Input(Iden).equals(400).help("Number of algae individuals pr m cultivation line");
    Output(Y).help("Dryweight of the algae (g)");
}

void Yieldprm::reset() {
   update();
}

void Yieldprm::update() {
   Y = (Ww * Iden)/1000;
}


}
