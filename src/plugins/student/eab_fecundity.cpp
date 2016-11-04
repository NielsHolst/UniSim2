#include <base/publish.h>
#include "eab_fecundity.h"

using namespace base;

namespace student {

PUBLISH(EabFecundity)

EabFecundity::EabFecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(ashState);
    Output(perCapita);
}

void EabFecundity::update() {
    perCapita = 50. + 10.*ashState;
}


}
