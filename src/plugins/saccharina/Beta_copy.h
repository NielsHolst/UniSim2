#ifndef BETA_COPY_H
#define BETA_COPY_H
#include <base/box.h>

namespace saccharina {

class Beta_copy : public base::Box
{
public:
    Beta_copy(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double alpha, Isat, maxphotosynthesisreftemp, beta1;
    // Outputs
    double beta_copy;
};

}

#endif
