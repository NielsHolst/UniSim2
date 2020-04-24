#ifndef GROSS_PHOTOSYNTHESIS_H
#define GROSS_PHOTOSYNTHESIS_H
#include <base/box.h>

namespace saccharina {

class Grossphotosynthesis : public base::Box
{
public:
    Grossphotosynthesis(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double PS, alpha, I, beta;
    // Outputs
    double Gphotosynthesis;
};

}

#endif
