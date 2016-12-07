#include <base/publish.h>
#include "output_buffer.h"

using namespace base;

namespace boxes {

PUBLISH(OutputBuffer)

OutputBuffer::OutputBuffer(QString name, QObject *parent)
    : OutputPorts(name, parent)
{
    help("keeps output in memory buffer");
}

}
