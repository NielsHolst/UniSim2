#ifndef APH_INTRINSIC_MORTALITY_H
#define APH_INTRINSIC_MORTALITY_H
#include <base/box.h>

namespace aphid {

class Aph_intrinsic_mortality : public base::Box
{
public:
    Aph_intrinsic_mortality(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    // Outputs
    double intrinsic_mortality;
};

}

#endif
