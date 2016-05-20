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
    Input(input3).page("default");
    Input(input4).page("default");
    Output(output1).page("default");
    Output(output2).page("default");
    Output(output3);
    Output(output4);
}

}
