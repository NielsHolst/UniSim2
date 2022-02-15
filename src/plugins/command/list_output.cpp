/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command.h>
#include <base/computation.h>
#include <base/dialog.h>
#include <base/port.h>
#include <base/port_type.h>
#include "list_output.h"

using namespace base;

namespace command {

ListOutput::ListOutput(QVector<Box*> boxes, ListOptionSet options)
    : _boxes(boxes), _options(options)
{
    _listInputs  = _options.contains(ListOption::Ports) ||
                   _options.contains(ListOption::Inputs);
    _listOutputs = _options.contains(ListOption::Ports) ||
                   _options.contains(ListOption::Outputs);
    _listImports = _options.contains(ListOption::Imports);
    _listExports = _options.contains(ListOption::Exports);
    _listShort   = _options.contains(ListOption::Short);
    _recurse     = _options.contains(ListOption::Recurse);
    _listExportsOnly = _listExports && !(_listInputs || _listOutputs || _listImports);

    try {
        if (Computation::currentStep() == Computation::Step::Amend) {
            Box::root()->initializeFamily();
            Box::root()->resetFamily();
            Box::root()->registerImportPortsFamily();
            dialog().message("Ready");
        }
    }
    catch (Exception &ex) {
        dialog().error(ex.what());
    }
    catch (...) {
        dialog().error("Something went wrong in command::ListOutput::ListOutput");
    }
}

QString ListOutput::toString() {
    for (Box *box : _boxes)
        toString(box, 0);
    return _s;
}

void ListOutput::toString(base::Box *box, int level) {
    // Always list box
    _s += QString().fill(' ', 2*level) +
          box->className() + " " +
          box->objectName() + "\n";
    // Sort ports
    QVector<Port*> ports = box->portsInOrder(),
                   inputs, outputs, aux,
                   inputImports, outputImports, auxImports;
    for (Port *port : ports) {
        switch (port->type()) {
        case PortType::Input:
            inputs << port;
            inputImports << port->importPorts();
            break;
        case PortType::Output:
            outputs << port;
            outputImports << port->importPorts();
            break;
        case PortType::Auxiliary:
            aux << port;
            auxImports << port->importPorts();
            break;
        }
    }

    // List ports
    if (_listInputs) for (Port *port : inputs) toString(port, level+1);
    else if (_listImports) for (Port *port : inputImports) toString(port, level+1);

    if (_listOutputs) for (Port *port : outputs) toString(port, level+1);
    else if (_listImports) for (Port *port : outputImports) toString(port, level+1);

    if (_listExportsOnly) {
        for (Port *port :  inputs) toString(port, level+1);
        for (Port *port : outputs) toString(port, level+1);
    }

    // List children
    if (_recurse)
        for (Box *child : box->findMany<Box*>("./*"))
            toString(child, level+1);
}

inline QStringList portNames(QVector<Port*> ports) {
    QStringList list;
    for (auto port : ports)
        list << port->fullName();
    return list;
}

void ListOutput::toString(base::Port *port, int level) {
    QMap<PortType, QString> prefix = {
        {PortType::Input    , "."},
        {PortType::Output   , ">"},
        {PortType::Auxiliary, "&"}
    };

    QString fill = QString().fill(' ', 2*level);

    if (!_listExportsOnly || !port->exportPorts().isEmpty()) {
        QString assignment = port->value().asString();
//        if (port->value().type() == Value::Type::String) {
//            if (assignment.size() > 13)
//                assignment = assignment.left(10) + "...";
//        }
        if (!_listShort) {
            if (!port->unit().isEmpty())
                assignment += " " + port->unit();
            if (!port->importPorts().isEmpty())
                assignment += " <- " + portNames(port->importPorts()).join("|");
        }
        _s += fill +
              prefix.value(port->type()) +
              port->objectName() +
              " = " +
              assignment +
              "\n";
    }

    if (_listExports && !_listShort) {
        for (Port *importer : port->exportPorts())
            _s += fill + "  >> " + importer->fullName() + "\n";
    }
}

}
