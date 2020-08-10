#ifndef TAYLOR_DEV_RATE_H
#define TAYLOR_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class TaylorDevRate : public base::Box
{
public:
    TaylorDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, Rm, Tm, T0 ;

    // Data
    double devrate;
};

}

#endif
