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
