#ifndef MEDIATOR_H
#define MEDIATOR_H
#include <QVector>
#include <base/box.h>
#include <base/matrix.h>

namespace student {

class Mediator : public base::Box
{
public:
    Mediator(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Data
    QVector<base::Box*> plants, pollinators;
    int numPlants, numPollinators;
    // From plant
    QVector<double>
        R,
    // From pollinator
        x, y, B;
    // Interactions
    base::Matrix<double> cr;

    // Inputs
    double
        preference, B0, h;

    // Methods
    double compute_F();
};

}

#endif
