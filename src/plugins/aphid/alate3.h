#ifndef ALATE3_H
#define ALATE3_H
#include <base/box.h>

namespace aphid {

class Alate3 : public base::Box
{
public:
    Alate3(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double (density);
    double percentage_senescence;
    double a1, b1, a2, b2;
    // Outputs
    double proportion;

};

}

#endif
