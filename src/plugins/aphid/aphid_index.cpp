#include <base/publish.h>
#include "aphid_index.h"

using namespace base;

namespace aphid {

PUBLISH(AphidIndex)

AphidIndex::AphidIndex(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates Entwistle-Dixon-Wratten density index ");
    Input(nymphs).help("Aphid nymph density").unit("per tiller");
    Input(adults).help("Aphid adult density").unit("per tiller");
    Output(value).help("Index value");
}

void AphidIndex::reset() {
    update();
}

void AphidIndex::update() {
    value = log(0.5*nymphs + adults + 0.01) ;
}

}
