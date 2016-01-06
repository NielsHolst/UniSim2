#include <iostream>
#include "box.h"
#include "exception.h"
#include "path.h"
#include "port.h"


namespace boxes {

Box *Box::_currentRoot = 0;
bool Box::_currentRootIsDirty = true;

Box::Box(QString name, QObject *parent)
    : QObject(parent), _name(name), _amended(false)
{
    setObjectName(name);
    _currentRoot = this;
    _currentRootIsDirty = true;

    Class(Box);
    Input(iterations).equals(1);
    Input(steps).equals(1);
    Output(iteration).doReset(false);
    Output(step);
}

Box::~Box() {
    if (this == _currentRoot)
        _currentRoot = 0;
}

void Box::addPort(Port *port) {
    QString name{port->name()};
    if (_ports.contains(name))
        throw Exception("Box already has a port with this name", name, this);
    _ports[name] = port;
}

Port* Box::port(QString name) {
    Path path(".[" + name + "]", this);
    QObjectList ports = (path.resolve());
    switch (ports.size()) {
    case 0: return 0;
    case 1: return dynamic_cast<Port*>(ports.at(0));
    default:
        throw Exception("Port name not unique within box", name, this);
    }
}

void Box::run() {
    std::cout << "run A\n";
    if (!_amended) {
        doAmend();
        _amended = true;
    }
    doInitialize();
    std::cout << "run B\n";
    for (iteration = 0; iteration < iterations; ++iteration) {
        doReset();
        std::cout << "run C" << iteration << " " << iterations << "\n";
        for (step = 0; step < steps; ++step) {
            doUpdate();
        }
        doCleanup();
    }
    doDebrief();
    std::cout << "run Z\n";
}

Box* Box::currentRoot() {
    if (!_currentRootIsDirty || _currentRoot==0)
        return _currentRoot;
    Box *p = _currentRoot;
    while (dynamic_cast<Box*>(p->parent()))
        p = dynamic_cast<Box*>(p->parent());
   return p;
}

void Box::doInitialize() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->doInitialize();
    }
    initializePorts();
    updateImports();
    initialize();
}

void Box::doReset() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->doReset();
    } \
    resetPorts();
    updateImports();
    reset();
}

#define RECURSE(method, doMethod) \
    void Box::doMethod() { \
        for (auto child : children()) { \
            Box *box = dynamic_cast<Box*>(child); \
            if (box) \
                box->doMethod(); \
        } \
        updateImports(); \
        method(); \
    }

RECURSE(amend, doAmend)
//RECURSE(initialize,doInitialize)
//RECURSE(reset, doReset)
RECURSE(update, doUpdate)
RECURSE(cleanup, doCleanup)
RECURSE(debrief, doDebrief)

void Box::updateImports() {

}

void Box::resetPorts() {
    std::cout << "resetPorts A\n";
    for (Port *port : _ports.values()) {
        if (port->doReset()) port->reset();
    }
    std::cout << "resetPorts Z\n";
}

void Box::initializePorts() {
    std::cout << "initializePorts A\n";
    for (Port *port : _ports.values()) {
        port->reset();
    }
    std::cout << "initializePorts Z\n";
}
}
