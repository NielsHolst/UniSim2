#ifndef TOTALCARBON_H
#define TOTALCARBON_H
#include <base/box.h>

namespace saccharina {

class Totalcarbon : public base::Box
{
public:
    Totalcarbon(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double C, Cstruct, Ws;
    // Outputs
    double Ctotal;
};

}

#endif
