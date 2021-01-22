#include <base/publish.h>
#include "model_a.h"

using namespace base;

namespace test_boxes {

PUBLISH(ModelA)

ModelA::ModelA(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(input1);
    Input(input2);
    Input(input3);
    Input(input4);
    Output(output1);
    Output(output2);
    Output(output3);
    Output(output4);
}

}
