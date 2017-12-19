#ifndef FOOD_WEB
#define FOOD_WEB
#include <base/box.h>
#include <base/matrix.h>

namespace boxes {

class FoodWeb : public base::Box
{
public: 
    FoodWeb(QString name, QObject *parent);
    void amend();
    void update();
private:
    // Inputs
    QString attackFile, gainFile;
    bool showMatrices;
    double timeStep;
    // Data
    int _n;
    QVector<base::Box*> _boxes;
    QVector<const double *> _densities, _demands;
    base::Matrix<double> _a, _g, _sApprox, _s, _acqApprox, _acq;
    QVector<double> _sPooled, _acqPooled;
    struct Outcome {
        double supply, loss,
            sdRatio, lossRatio;
    };
    QVector<Outcome> _outcomes;

    // Methods
    void setupOutputs();
    double N(int i) const { return *_densities.at(i); }
    double D(int i) const { return *_demands.at(i); }
    void update_sPooled();
    void update_sApprox();
    void update_s();
    void update_acqPooled();
    void update_acqApprox();
    void update_acq();
    void update_losses();
    void update_supplies();

};

} //namespace
#endif
