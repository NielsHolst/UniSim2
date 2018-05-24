#ifndef CROP_GROWS_H
#define CROP_GROWS_H
#include <base/box.h>

namespace aphid {

class Crop_grows : public base::Box
{
public:
    Crop_grows(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Temperature;
    double Date;
    double Starting_date;
    // Outputs
    bool Growth;
    // Data
    double Temperature_2;
    double Temperature_3;
    double Temperature_4;
    double Temperature_5;
};

}

#endif
