/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
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
    _recurse     = _options.contains(ListOption::Recurse);
    _listExportsOnly = _listExports && !(_listInputs || _listOutputs || _listImports);

    try {
        ComputationStep step = environment().computationStep();
        if (step == ComputationStep::Amend) {
            environment().root()->initializeFamily();
            environment().root()->resetFamily();
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
    QVector<Port*> ports = box->findMany<Port>("./*<Port>"),
                   inputs, outputs,
                   inputImports, outputImports;
    for (Port *port : ports) {
        switch (port->access()) {
        case PortAccess::Input:
            inputs << port;
            if (port->hasImport()) inputImports << port;
            break;
        case PortAccess::Output:
            outputs << port;
            if (port->hasImport()) outputImports << port;
            break;
        case PortAccess::Uninitialized:
            ThrowException("Uninitialized port access");
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
        for (Box *child : box->findMany<Box>("./*<Box>"))
            toString(child, level+1);
}

void ListOutput::toString(base::Port *port, int level) {
    QString fill = QString().fill(' ', 2*level);

    if (!_listExportsOnly || !port->exportPorts().isEmpty()) {
        QString prefix = port->isBlind() ? "+" :
                        (port->access()==PortAccess::Input) ? "." : "~",
                assignment = port->valueAsString();
        if (port->type() == Char || port->type() == String) {
//            if (assignment.size() > 13)
//                assignment = assignment.left(10) + "...";
            assignment = "\"" + assignment + "\"";
        }
        if (!port->unit().isEmpty())
            assignment += " " + port->unit();
        if (port->hasImport())
            assignment += " <- " + port->importPath();
        _s += fill +
              prefix +
              port->objectName() +
              " = " +
              assignment +
              "\n";
    }

    if (_listExports) {
        for (Port *importer : port->exportPorts())
            _s += fill + "  >> " + importer->fullName() + "\n";
    }
}

}
