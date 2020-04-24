#include <base/publish.h>
#include "Nitrogen_content.h"

using namespace base;

namespace saccharina {

PUBLISH(Nitrogencontent)

Nitrogencontent::Nitrogencontent(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the nitrogen content after a certain timestep (g N (g sw)^-1)");
    Input(Ninit).equals(0.022).help("Nitrogen content at deployment");
    Input(nitrogenreservesrate).equals(0.02).help("Rate of change of nitrogen reserves");
    Input(dt).equals(1.0).help("timestep (days)");
    Output(N).help("Nitrogen content (g N (g sw)^-1)");
}

void Nitrogencontent::reset() {
   N = Ninit;
}

void Nitrogencontent::update() {
    N = N * nitrogenreservesrate * ((dt/3600)/24);
}


}
