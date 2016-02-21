#ifndef SIMULATION_H
#define SIMULATION_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class Simulation : public base::Box
{
public:
    Simulation(QString name, QObject *parent);
    void initialize();
    void run();
private:
    // Inputs
    int iterations, steps;
    // Outputs
    int iteration, step;
    // Data
    QVector<base::Port*> _trackedPorts;
    // Methods
    void collectTrackedPorts();
    void makePortLabelsUnique();
};

}

#endif
