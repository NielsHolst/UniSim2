#ifndef RATE_OF_CHANGE_IN_FROND_AREA_H
#define RATE_OF_CHANGE_IN_FROND_AREA_H
#include <base/box.h>

namespace saccharina {

class Rateofchangeoffrondarea : public base::Box
{
public:
    Rateofchangeoffrondarea(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double u, fronderosion, A;
    // Outputs
    double frondgrowthrate;
};

}

#endif
