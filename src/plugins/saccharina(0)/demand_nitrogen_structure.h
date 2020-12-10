#ifndef DEMAND_NITROGEN_STRUCTURE_H
#define DEMAND_NITROGEN_STRUCTURE_H
#include <base/box.h>

namespace saccharina {

class DemandNitrogenStructure : public base::Box
{
public:
    DemandNitrogenStructure(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double proportionN, proportionC, demandCarbonStructure;
    // Outputs
    double value;
};

}

#endif
