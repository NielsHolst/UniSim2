#ifndef WHEAT_GROWTH_AG_H
#define WHEAT_GROWTH_AG_H
#include <base/box.h>

namespace aphid {

class Wheat_growth_AG : public base::Box
{
public:
    Wheat_growth_AG(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Degree_days;
    double Min; // value that varies according to the sowing date based on the data from Ås 2016, Ås 2017 and Ramnes 2013
    double ED50; // value that varies according to the sowing date based on the data from Ås 2016, Ås 2017 and Ramnes 2013
    // Outputs
    double Pheno_stage;
    // Data
    double upper_limit=90; // maximum value of the phenological stage
    double slope=-4.2; // slope of the logistic function
};

}

#endif
