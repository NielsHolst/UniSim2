#ifndef AREALOST_H
#define AREALOST_H
#include <base/box.h>

namespace saccharina {

class Arealost : public base::Box
{
public:
    Arealost(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A, C, Cmin, Cstruct;
    // Outputs
    double Alost;
};

}

#endif
