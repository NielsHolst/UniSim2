#ifndef SIMULATION_H
#define SIMULATION_H
#include <QFile>
#include <QTextStream>
#include <QList>
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class Simulation : public base::Box
{
public:
    Simulation(QString name, QObject *parent);
    void amend();
    void initialize();
    void debrief();
    void run();
private:
    // Inputs
    int iterations, steps;
    // Outputs
    int iteration, step;
    // Data
    QList<base::Port*> _trackedPorts;
    QFile _file;
    QTextStream _stream;
    // Methods
    void collectTrackedPorts();
    void removeObsoletePorts();
    void makePortLabelsUnique();
    void writeDataFrame();
    void openFileStream();
    static bool valuesAreEqual(const base::Port *port);
};

}

#endif
