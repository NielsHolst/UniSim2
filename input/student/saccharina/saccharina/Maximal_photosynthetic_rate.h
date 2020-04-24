#ifndef MAXIMAL_PHOTOSYNTHETIC_RATE_H
#define MAXIMAL_PHOTOSYNTHETIC_RATE_H
#include <base/box.h>

namespace saccharina {

class Maximalphotosyntheticrate : public base::Box
{
public:
    Maximalphotosyntheticrate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double alpha, Isat, beta;
    // Outputs
    double maxphotosynthesis;
};

}

#endif
