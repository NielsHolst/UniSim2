#include <base/publish.h>
#include "model_a.h"

using namespace base;

namespace test {

PUBLISH(ModelA)

ModelA::ModelA(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(input1);
    Input(input2);
    Input(input3).track();
    Input(input4).track();
    Output(output1).track();
    Output(output2).track();
    Output(output3);
    Output(output4);
}

}
