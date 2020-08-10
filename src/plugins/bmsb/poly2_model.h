#ifndef POLY2_MODEL_H
#define POLY2_MODEL_H
#include <base/box.h>

namespace bmsb {

class Poly2Model : public base::Box
{
public:
    Poly2Model(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1, a2;

    // Data
    double p2moutput;
};

}

#endif
