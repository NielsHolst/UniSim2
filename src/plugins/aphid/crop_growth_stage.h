#ifndef CROP_GROWTH_STAGE_H
#define CROP_GROWTH_STAGE_H
#include <base/box.h>

namespace aphid {

class CropGrowthStage : public base::Box
{
public:
    CropGrowthStage(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dayDegrees, valueAtStart, dayDegreesHalfways, slope;
    // Outputs
    double value;
};

}

#endif
