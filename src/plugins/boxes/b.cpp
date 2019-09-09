/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "b.h"

using namespace base;

namespace boxes {

PUBLISH(B)

B::B(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes Saltelli et al. B function");
    Input(x1).help("X1 input");
    Input(x2).help("X2 input");
    Input(x3).help("X3 input");
    Input(x4).help("X4 input");
    Input(x5).help("X5 input");
    Input(omega1).help("omega1 input");
    Input(omega2).help("omega2 input");
    Input(omega3).help("omega3 input");
    Input(omega4).help("omega4 input");
    Input(omega5).help("omega5 input");
    Output(value).help("B value");
}


void B::reset() {
  update();
}

void B::update() {
  value =
    x1*omega1 +
    x2*omega2 +
    x3*omega3 +
    x4*omega4 +
    x5*omega5;
}

}
