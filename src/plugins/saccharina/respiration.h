#ifndef RESPIRATION_H
#define RESPIRATION_H
#include <base/box.h>

namespace saccharina {

class Respiration : public base::Box
{
public:
    Respiration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double r1, TAR, TR1, T;
    // Outputs
    double value;
};

}

#endif
