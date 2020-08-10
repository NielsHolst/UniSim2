#include <base/publish.h>
#include "Carbon_content.h"

using namespace base;

namespace saccharina {

PUBLISH(Carboncontent)

Carboncontent::Carboncontent(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the carbon content after a certain timestep (g C (g sw)^-1)");
    Input(Cinit).equals(0.2).help("Carbon content at deployment");
    Input(carbonreservesrate).equals(0.02).help("The rate of change in carbon reserves");
    Input(dt).imports("calendar[timeStepSecs]");
    Input(Cmin).equals(0.01).help("Minimal carbon reserve");
    Output(C).help("Carbon content (g C (g sw)^-1)");
}

void Carboncontent::reset() {
   C = Cinit;
}

void Carboncontent::update() {
    C += C * carbonreservesrate * (dt/3600);
    }
}
