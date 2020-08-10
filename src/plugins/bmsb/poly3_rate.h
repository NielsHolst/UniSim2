#ifndef POLY3_RATE_H
#define POLY3_RATE_H
#include <base/box.h>

namespace bmsb {

class Poly3Rate : public base::Box
{
public:
    Poly3Rate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1, a2, a3  ;

    // Data
    double survrate;
};

}

#endif
