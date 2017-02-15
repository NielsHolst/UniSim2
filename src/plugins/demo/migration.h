#ifndef MIGRATION_H
#define MIGRATION_H
#include <QVector>
#include <base/matrix.h>
#include <base/box.h>

namespace demo {

class Migration : public base::Box
{
public:
    Migration(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Data
    QVector<Box*> populations;      // Populations connected by migration
    int n;                          // Number of populations
    base::Matrix<double> distances, // Distances between populations (nxn)
                         emRates;   // Each row holds emigration rates from one pop to all other pops (nxn)

    struct Outcome {
        double em;                  // Proportion of population emigrating;
                                    // goes to population's .phaseOutflowProportion
        QVector<double> im;         // Immigrants to population
                                    // goes to population's .phaseInflow
    };

    QVector<Outcome> outcomes;      // One outcome for each population

    // Methods
    void setupOutcomePorts();
    void setupDistances();
    void updateEmigration();
    double computeEmigration(double distance, double slope) const;
    void updateImmigration();
};

}

#endif
