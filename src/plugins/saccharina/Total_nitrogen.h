#ifndef TOTALNITROGEN_H
#define TOTALNITROGEN_H
#include <base/box.h>

namespace saccharina {

class Totalnitrogen : public base::Box
{
public:
    Totalnitrogen(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double N, Nstruct, Ws;
    // Outputs
    double Ntotal;
};

}

#endif
