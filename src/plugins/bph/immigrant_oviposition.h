#ifndef IMMIGRANT_OVIPOSITION_H
#define IMMIGRANT_OVIPOSITION_H
#include <base/box.h>

namespace bph {

class ImmigrantOviposition : public base::Box
{
public:
    ImmigrantOviposition(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double vegetative, immigrants, fecundity;
    // Data
    double value;
};

}

#endif
