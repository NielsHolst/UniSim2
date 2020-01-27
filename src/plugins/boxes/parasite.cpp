/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/publish.h>
#include "parasite.h"
using namespace base;

namespace boxes {

PUBLISH(Parasite)

Parasite::Parasite(QString name, QObject *parent)
    : Box(name, parent) {
    help("holds the demand and supply of a parasite");
    Input(density).help("Density");
    Input(demand).help("Amount of resource needed");
    Input(supplyTotal).help("Total amount of resource acquired");
    Input(supplyFromHost).help("Amount of resource acquired from each prey");
    Input(sdRatio).help("Total supply/demand ratio").unit("[0;1]");
}

void Parasite::initialize() {
    QVector<Box*> hosts = findMany<Box>("./*<Host>");
    supplyFromHost.fill(0, hosts.size());
    QString s = "Parasite %1 has %2 host(s)";
    dialog().information(s.arg(objectName()).arg(hosts.size()));
}

} //namespace

