#include <iostream>
#include "box.h"
#include "exception.h"
#include "general.h"
#include "path.h"
#include "port.h"

namespace boxes {

Box *Box::_currentRoot = 0;
bool Box::_currentRootIsDirty = true;

Box::Box(QString name, QObject *parent)
    : QObject(parent), _name(name), _amended(false)
{
    Class(Box);
    setObjectName(name);
    _currentRoot = this;
    _currentRootIsDirty = true;
}

Box::~Box() {
    if (this == _currentRoot)
        _currentRoot = 0;
}

void Box::addPort(Port *port) {
    QString name{port->objectName()};
    if (_ports.contains(name))
        throw Exception("Box already has a port with this name", name, this);
    _ports[name] = port;
}

Port* Box::port(QString name) {
    Path path(".[" + name + "]", this);
    auto ports = (path.resolve());
    switch (ports.size()) {
    case 0:  throw Exception("No port of that name in this box", name, this);
    case 1:  return dynamic_cast<Port*>(ports.at(0));
    default: throw Exception("Port name not unique within box", name, this);
    }
}

Box* Box::currentRoot() {
    if (!_currentRootIsDirty || _currentRoot==0)
        return _currentRoot;
    Box *p = _currentRoot;
    while (dynamic_cast<Box*>(p->parent()))
        p = dynamic_cast<Box*>(p->parent());
   return p;
}

void Box::run() {
    throw Exception("Method 'run' not defined for this class", className(this), this);
}

void Box::amendFamily() {
    if (_amended) return;
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->amendFamily();
    }
    amend();
    _amended = true;
}

void Box::initializeFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->initializeFamily();
    }
    resolvePorts();
    updateImports();
    initialize();
}

void Box::resetFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->resetFamily();
    }
    resetPorts();
    updateImports();
    reset();
}

#define RECURSE(method) \
    void Box::method##Family() { \
        for (auto child : children()) { \
            Box *box = dynamic_cast<Box*>(child); \
            if (box) \
                box->method##Family(); \
        } \
        updateImports(); \
        method(); \
    }

RECURSE(update)
RECURSE(cleanup)
RECURSE(debrief)

void Box::resolvePorts() {
    for (Port *port : _ports.values())
        port->resolveImports();
}

void Box::updateImports() {
    for (Port *port : _ports.values())
        port->copyFromImport();
}

void Box::resetPorts() {
    for (Port *port : _ports.values())
        port->reset();
}


}
