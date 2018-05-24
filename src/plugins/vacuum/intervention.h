#ifndef  INTERVENTION_H
#define  INTERVENTION_H
#include <base/box.h>

namespace vacuum {

class Intervention : public base::Box
{
public:
     Intervention(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    int dayValue;
    int interval;
    double efficacy;
    // Outputs
    double mortality;
};

}

#endif
