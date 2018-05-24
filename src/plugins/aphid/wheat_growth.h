#ifndef WHEAT_GROWTH_H
#define WHEAT_GROWTH_H
#include <base/box.h>

namespace aphid {

class Wheat_growth : public base::Box
{
public:
    Wheat_growth(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Degree_days;
    // Outputs
    double Pheno_stage;
    // Data
    double theta1=-1.9427;
    double theta2=-0.0031;
    double theta3=95.2895;
};

}

#endif
