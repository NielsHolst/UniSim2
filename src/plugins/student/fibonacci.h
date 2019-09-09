/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FIBONACCI_H
#define FIBONACCI_H
#include <base/box.h>

namespace student {

class Fibonacci : public base::Box
{
public:
    Fibonacci(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Outputs
    int value;
    // Data
    int previous;
};

}

#endif
