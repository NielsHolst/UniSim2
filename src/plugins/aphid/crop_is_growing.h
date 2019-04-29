#ifndef CROP_IS_GROWING_
#define CROP_IS_GROWING_H
#include <base/box.h>

namespace aphid {

class CropIsGrowing : public base::Box
{
public:
    CropIsGrowing(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double temperature;
    // Outputs
    bool value;
    // Data
    double temperature_2, temperature_3, temperature_4, temperature_5;
};

}

#endif
