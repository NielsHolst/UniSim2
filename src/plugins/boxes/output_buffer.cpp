/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "output_buffer.h"

using namespace base;

namespace boxes {

PUBLISH(OutputBuffer)

OutputBuffer::OutputBuffer(QString name, QObject *parent)
    : OutputPorts(name, parent)
{
    Class(OutputBuffer);
    help("keeps output in a memory buffer");
}

}
