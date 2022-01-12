/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_PORTS_H
#define OUTPUT_PORTS_H
#include <base/box.h>
#include <base/path.h>

namespace boxes {

class OutputPorts : public base::Box
{
public:
    OutputPorts(QString name, Box *parent);
protected:
    // Inputs
    base::Path ports;
};

}

#endif
