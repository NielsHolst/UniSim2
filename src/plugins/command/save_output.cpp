#include <base/box.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "save_output.h"

using namespace base;

namespace command {

PUBLISH(save_output)

save_output::save_output(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(save_output);
}

void save_output::execute() {
    Box *root = environment().state.root;
    if (_args.size() > 2) {
        dialog().error("Command 'save output' takes no arguments");
    }
    else if (root) {
        if (collectPortsWithTrack()) {
            dialog().information(QString::number(_ports.size()) + " tracked ports");
            QString string;
            QTextStream output(&string);
            if (writeTracks(output))
                dialog().information(string);
        }
    }
    else
        dialog().error("No box loaded");
}

bool save_output::collectPortsWithTrack() {
    _ports.clear();
    Path path("*{Port}", this);
    QVector<Port*> all;
    try {
        all = path.resolve<Port>(-1, this);
    }
    catch (Exception &ex) {
        dialog().error(QString("Unexpected error: ") + ex.what());
        return false;
    }
    for (Port *port : all) {
        if (port->trackPtr())
            _ports << port;
    }
    return true;
}

bool save_output::writeTracks(QTextStream &output) {

    // Check ports
    if (_ports.isEmpty()) {
        dialog().error("No ports are being tracked");
        return false;
    }

    // Check number of rows
    int nrow = _ports.at(0)->trackPtr()->size();
    for (Port *port : _ports) {
        Q_ASSERT(port->trackPtr()->size() == nrow);
    }
    Port *last = _ports.last();

    // Write columns labels
    for (Port *port : _ports) {
        output << port->objectName();
        if (port != last)
            output << "\t";
    }
    output << "\n";

    // Write columns values as text
    for (int row = 0; row < nrow; ++row) {
        for (Port *port : _ports) {
            output << port->trackPtr()->toString(row);
            if (port != last)
                output << "\t";
        }
        if (row < nrow-1)
            output << "\n";
    }
    return true;
}

}
