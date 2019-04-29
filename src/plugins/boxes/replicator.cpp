#include <base/publish.h>
#include "replicator.h"

using namespace base;

namespace boxes {

PUBLISH(Replicator)

Replicator::Replicator(QString name, QObject *parent)
    : Box(name, parent)
{
    help("replicates simulation runs");
    Input(step).imports("/*[step]");
    Input(steps).imports("/*[steps]");
    Input(replicates).equals(1).help("Number of replicates").unit("1,2,3,...");
    Output(replicate).help("Replicate number").unit("0,1,2,...");
}


void Replicator::reset() {
    replicate = 0;
}

void Replicator::update() {
    ++replicate;
}

}
