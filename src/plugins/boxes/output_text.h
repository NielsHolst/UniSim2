#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H
#include <QFile>
#include <QList>
#include <QTextStream>
#include "output_ports.h"

namespace base {
    class Port;
}

namespace boxes {

class OutputText : public OutputPorts
{
public:
    OutputText(QString name, QObject *parent);
    void initialize();
    void debrief();
private:
    // Data
    QFile _file;
    QTextStream _stream;
    QList<base::Port*> _allTrackedPorts;
    // Methods
    void collectAllTrackedPorts();
    void makePortLabelsUnique();
    void writeDataFrame();
    void removeObsoletePorts();
    void openFileStream(QString extension);
    static bool valuesAreEqual(const base::Port *port);
};

} // boxes

#endif
