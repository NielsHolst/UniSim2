#ifndef APH_SURVIVALD_H
#define APH_SURVIVALD_H
#include <base/box.h>

namespace aphid {

class Aph_survivalD : public base::Box
{
public:
    Aph_survivalD(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    double Temperature;
    // Outputs
    double survival;
};

}

#endif
