#ifndef MEDIATOR_H
#define MEDIATOR_H
#include <QVector>
#include <base/box.h>
#include <base/matrix.h>

namespace pollnet {

class Mediator : public base::Box
{
public:
    Mediator(QString name, QObject *parent);
    void amend();
    void initialize();
    void reset();
    void update();
private:
    // Data
    QVector<base::Box*> plants, pollinators;
    int numPlants, numPollinators;
    double B0_h;
    // From plant
    QVector<double>
        R, V,
    // From pollinator
        x, y, B,
    // Interactions
        Gain_Pollinator, Loss_Floral, Poll_Benefit;

    base::Matrix<double> CR;

    // Inputs
    double B0, h;
    QString connectivityFile;
    base::Matrix<double> preference;

    // Output
    double Vsum;

    // Methods
    double compute_F(int i, int j);
    double compute_Vsum();
    void setup_Consumption_Ports();
};

}

#endif
