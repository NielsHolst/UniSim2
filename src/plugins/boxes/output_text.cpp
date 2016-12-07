#include <base/dialog.h>
#include <base/environment.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/unique_name.h>
#include "output_text.h"

using namespace base;

namespace boxes {

PUBLISH(OutputText)

OutputText::OutputText(QString name, QObject *parent)
    : OutputPorts(name, parent)
{
    help("creates an output text file");
}

void OutputText::initialize() {
    collectAllTrackedPorts();
    makePortLabelsUnique();
}

void OutputText::collectAllTrackedPorts() {
    _allTrackedPorts.clear();
    QVector<Port*> all = Path("*{Port}").resolveMany<Port>();
    for (Port *port : all) {
        if (port->trackPtr())
            _allTrackedPorts << port;
    }
}

void OutputText::makePortLabelsUnique() {
    UniqueName unique(_allTrackedPorts.toVector());
    QStringList labels = unique.resolve();
    int i{0};
    for (Port *port : _allTrackedPorts)  {
        QString label = labels.at(i++);
        label.replace("/", ".");
        port->label(label);
    }
}

void OutputText::debrief() {
    openFileStream(".txt");
    writeDataFrame();
    dialog().information("Data frame written to '" + environment().latestOutputFilePath("txt") + "'");
    _file.close();
}

void OutputText::openFileStream(QString extension) {
    environment().openOutputFile(_file, extension);
    _stream.setDevice(&_file);
}

void OutputText::writeDataFrame() {
    // Check ports
    if (_allTrackedPorts.isEmpty())
        ThrowException("No ports are being tracked");

    removeObsoletePorts();

    // Check number of rows
    int nrow = _allTrackedPorts.at(0)->trackPtr()->size();
    for (Port *port : _allTrackedPorts) {
        Q_ASSERT(port->trackPtr()->size() == nrow);
    }

    // Write column labels
    QStringList list;
    for (Port *port : _allTrackedPorts)
        list << port->labelList();
    _stream << list.join("\t") << "\n";

    // Write column format
    list.clear();
    for (Port *port : _allTrackedPorts) {
        for (int i = 0; i < port->valueSize(); ++i)
           list << port->format();
    }
    _stream << list.join("\t") << "\n";

    // Write column values as text
    Port *last = _allTrackedPorts.last();
    for (int row = 0; row < nrow; ++row) {
        for (Port *port : _allTrackedPorts) {
            _stream << port->trackPtr()->toString(row);
            if (port != last)
                _stream << "\t";
        }
        if (row < nrow-1)
            _stream << "\n";
    }
}

void OutputText::removeObsoletePorts() {
    QList<Port*>::iterator itPort = _allTrackedPorts.begin();
    while(itPort != _allTrackedPorts.end()) {
        Port *port = *itPort;
        bool isFactor = port->format() == "factor";
        bool isUninformative = valuesAreEqual(port);
        if (isFactor && isUninformative)
            itPort = _allTrackedPorts.erase(itPort);
        else
            itPort++;
    }
}

bool OutputText::valuesAreEqual(const Port *port) {
    const Vector *column = port->trackPtr();
    int nrow = column->size();
    if (nrow == 0)
        return true;
    QString value = column->toString(0);
    for (int row = 1; row < nrow; ++row) {
        if (column->toString(row) != value)
            return false;
    }
    return true;
}

}
