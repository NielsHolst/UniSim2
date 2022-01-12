/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef B_H
#define B_H
#include <base/box.h>

namespace boxes {

class B : public base::Box
{
public:
    B(QString name, Node *parent);
    void reset();
    void update();
private:
    // Input
    double x1, x2, x3, x4, x5,
      omega1, omega2, omega3, omega4, omega5;
    // Output
    double value;
};

}

#endif
