#ifndef THRESHOLD_POP_H
#define THRESHOLD_POP_H
#include <base/box.h>

namespace aphid {

class Threshold_pop : public base::Box
{
public:
    Threshold_pop(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double fungal_mortality;
    double natural_survival;
    double transmission_capa;
    // Outputs
    double Ht;

};

}

#endif
