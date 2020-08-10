#ifndef NRR_EVF_H
#define NRR_EVF_H
#include <base/box.h>

namespace bmsb {

class NrrEVF : public base::Box
{
public:
    NrrEVF(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a, b, k;

    // Data
    double nrrout;
};

}

#endif
