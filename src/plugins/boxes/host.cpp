/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "host.h"
using namespace base;

namespace boxes {

PUBLISH(Host)

Host::Host(QString name, QObject *parent)
    : Box(name, parent) {
    help("holds the host parameters for interaction with the parent Parasite");
    Input(density).help("Density");
    Input(attackRate).help("Attack rate").unit("per parasite per time unit");
    Input(gainFactor).equals(1.).help("Conversion of resource into supply units").unit("supply/attacks");
    Input(numAttacks).help("Number of attacks effectuated by parent Parasite");
    Input(numHostAttacked).help("Number of hosts attacked by parent Parasite");
}

} //namespace

