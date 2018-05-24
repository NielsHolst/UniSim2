#ifndef APH_INTRINSIC_MORTALITYD_H
#define APH_INTRINSIC_MORTALITYD_H
#include <base/box.h>

namespace aphid {

class Aph_intrinsic_mortalityD : public base::Box
{
public:
    Aph_intrinsic_mortalityD(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    double Temperature;
    // Outputs
    double intrinsic_mortality;
};

}

#endif
