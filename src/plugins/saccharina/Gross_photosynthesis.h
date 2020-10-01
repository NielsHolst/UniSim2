#ifndef GROSS_PHOTOSYNTHESIS_H
#define GROSS_PHOTOSYNTHESIS_H
#include <base/box.h>

namespace saccharina {

class GrossPhotosynthesis : public base::Box
{
public:
    GrossPhotosynthesis(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double alpha, beta, I, Isat;
    // Outputs
    double value;
};

}

#endif
