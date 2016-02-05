#include <base/publish.h>
#include "model_a.h"

using namespace base;

namespace test_boxes {

PUBLISH(ModelA)

ModelA::ModelA(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(ModelA);
    // Default is off
    Input(input1);
    Input(input2);
    Input(input3).trackOn();
    Input(input4).trackOn();
    // Default is on
    Output(output1);
    Output(output2);
    Output(output3).trackOff();
    Output(output4).trackOff();
}

}
