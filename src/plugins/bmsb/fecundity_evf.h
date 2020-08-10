#ifndef FECUNDITY_EVF_H
#define FECUNDITY_EVF_H
#include <base/box.h>

namespace bmsb {

class FecundityEVF : public base::Box
{
public:
    FecundityEVF(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a, b, k;

    // Data
    double fecundity;
};

}

#endif
