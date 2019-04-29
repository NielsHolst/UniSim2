#include <base/publish.h>
#include "biocontrol.h"

using namespace base;

namespace aphid {

PUBLISH(Biocontrol)

Biocontrol::Biocontrol(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimates biocontrol effects");
    Input(aphidDaysUncontrolled).help("Pest pressure when uncontrolled").unit("aphid-days");
    Input(aphidDaysControlled).help("Pest pressure when controlled").unit("aphid-days");
    Input(yieldUncontrolled).help("Relative yield when uncontrolled").unit("[0;1]");
    Input(yieldControlled).help("Relative yield when controlled").unit("[0;1]");
    Output(aphidDaysImprovement).help("Improvement in pest control").unit("aphid-days");
    Output(yieldImprovement).help("Improvement in relative yield when controlled").unit("[0;1]");
}

void Biocontrol::update() {
    aphidDaysImprovement = aphidDaysUncontrolled - aphidDaysControlled;
    yieldImprovement = yieldControlled - yieldUncontrolled;
}

}
