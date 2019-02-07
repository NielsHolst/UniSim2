#ifndef PARAM_APHID_H
#define PARAM_APHID_H
#include <base/box.h>

namespace aphid {

class Param_aphid : public base::Box
{
public:
    Param_aphid(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double longevityImmigrant;
    double longevityAdult;
    double longevityNapt;
    double longevityNala;
};

}

#endif
