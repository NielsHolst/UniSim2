/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_BUFFER_H
#define OUTPUT_BUFFER_H
#include "output_ports.h"

namespace boxes {

class OutputBuffer : public OutputPorts
{
public:
    OutputBuffer(QString name, QObject *parent);
};

} // boxes

#endif
