#ifndef APH_SURVIVALP_H
#define APH_SURVIVALP_H
#include <base/box.h>

namespace aphid {

class Aph_survivalP : public base::Box
{
public:
    Aph_survivalP(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Wheat_Pheno;
    // Outputs
    double survival;
};

}

#endif
