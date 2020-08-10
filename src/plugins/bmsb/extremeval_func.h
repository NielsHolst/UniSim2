#ifndef EXTREMEVAL_FUNC_H
#define EXTREMEVAL_FUNC_H
#include <base/box.h>

namespace bmsb {

class ExtremeValFunc : public base::Box
{
public:
    ExtremeValFunc(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, k1, Tmax, rho;

    // Data
    double evfout;
};

}

#endif
