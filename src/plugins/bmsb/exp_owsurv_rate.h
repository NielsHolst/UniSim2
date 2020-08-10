#ifndef EXP_OWSURV_RATE_H
#define EXP_OWSURV_RATE_H
#include <base/box.h>

namespace bmsb {

class ExpOWSurvRate : public base::Box
{
public:
    ExpOWSurvRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a, b, c, t;

    // Data
    double owsurvrate;
};

}

#endif
