/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "projected_deqs.h"

using namespace base;

namespace awe {

PUBLISH(ProjectedDeqs)

ProjectedDeqs::ProjectedDeqs(QString name, QObject *parent)
    : Box(name,parent)
{
    Input(seedlingDeqs).imports("seedling/densityEqs[content]");
    Input(juvenileDeqs).imports("juveile/densityEqs[content]");
    Input(matureDeqs).imports("mature/densityEqs[content]");
    Input(matureDeqsOutflowTotal).imports("mature/densityEqs[outflowTotal]");
    Output(total);
}

void ProjectedDeqs::reset() {
    _prevOutflowTotal = 0.;
}

void ProjectedDeqs::update() {
    total = seedlingDeqs + juvenileDeqs + matureDeqs + matureDeqsOutflowTotal - _prevOutflowTotal;
}

}
