#ifndef BETA_RATE_H
#define BETA_RATE_H
#include <base/box.h>

namespace bmsb {

class BetaRate : public base::Box
{
public:
   BetaRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, mu, Tb, Tc, aa, bb ;

    // Data
    double ovprate;
};

}

#endif
