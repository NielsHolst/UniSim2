#ifndef ALATE2_H
#define ALATE2_H
#include <base/box.h>

namespace student {

class Alate2 : public base::Box
{
public:
    Alate2(QString name, QObject *parent);
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
