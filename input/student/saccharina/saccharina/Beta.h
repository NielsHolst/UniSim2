#ifndef BETA_H
#define BETA_H
#include <base/box.h>

namespace saccharina {

class Beta : public base::Box
{
public:
    Beta(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double alpha, Isat, maxphotosynthesisreftemp;
    // Outputs
    double beta;
};

}

#endif
