#ifndef CARBON_EXUDATION_H
#define CARBON_EXUDATION_H
#include <base/box.h>

namespace saccharina {

class Carbonexudation : public base::Box
{
public:
    Carbonexudation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double gamma, C, Cmin;
    // Outputs
    double carbonexudation;
};

}

#endif
