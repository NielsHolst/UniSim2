#ifndef MODEL_A_H
#define MODEL_A_H
#include <base/box.h>

namespace test {

class ModelA : public base::Box
{
public:
    ModelA(QString name, QObject *parent);
private:
    int input1, input2, input3, input4,
        output1, output2, output3, output4;
};

}

#endif
