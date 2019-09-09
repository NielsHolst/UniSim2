/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef JUMP_H
#define JUMP_H
#include <base/box.h>

namespace student {

class Jump : public base::Box
{
public:
    Jump(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    int initial;
    // Outputs
    int value;
    bool finished;
};

}

#endif
