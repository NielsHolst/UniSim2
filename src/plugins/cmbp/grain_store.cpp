#include <base/publish.h>
#include "grain_store.h"

using namespace base;

namespace cmbp {

PUBLISH(GrainStore)

GrainStore::GrainStore(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Tmin).help("Outdoors daily minimum temperature (oC)");
    Input(Tmax).help("Outdoors daily maximum temperature (oC)");
    Input(Toffset).help("Offset for temprature inside the store (oC)");
    Input(mass).equals(500).help("Mass of stored grain (kg)");
    Output(T).help("Store temperature (oC");
}

void GrainStore::reset() {
    update();
}

void GrainStore::update() {
    T = (Tmin + Tmax)/2. + Toffset;
}


}
