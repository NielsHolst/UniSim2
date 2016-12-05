#include <base/publish.h>
#include "output_text.h"

using namespace base;

namespace boxes {

PUBLISH(OutputText)

OutputText::OutputText(QString name, QObject *parent)
    : OutputPorts(name, parent)
{
    help("creates an output text file");
}

}
