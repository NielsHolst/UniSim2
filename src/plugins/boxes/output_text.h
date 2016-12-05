#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H
#include "output_ports.h"


namespace boxes {

class PageR;

class OutputText : public OutputPorts
{
public:
    OutputText(QString name, QObject *parent);
};

} // boxes

#endif
