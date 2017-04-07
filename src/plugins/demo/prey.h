#ifndef PREY_H
#define PREY_H
#include <QVector>
#include <base/box.h>

namespace demo {

class Prey : public base::Box
{
public:
    Prey(QString name, QObject *parent);
    void initialize();
private:
    // Inputs
    double r, K, totalPrey, loss;
    // Outputs
    double rActual;
};

}

#endif
