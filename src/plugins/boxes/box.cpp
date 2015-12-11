#include "box.h"
#include "exception.h"
#include "port.h"


namespace boxes {

Box *Box::_currentRoot = 0;
bool Box::_currentRootIsDirty = true;

Box::Box(QString name, QObject *parent)
    : QObject(parent), _name(name), amended(false)
{
    setObjectName(name);
    _currentRoot = this;
    _currentRootIsDirty = true;

    Class(Box);
    Input(iterations).equals(1);
    Input(steps).equals(1);
    Output(iteration);
    Output(step);
}

Box::~Box() {
    if (this == _currentRoot)
        _currentRoot = 0;
}

void Box::addPort(Port *port) {
    QString name{port->name()};
    if (ports.contains(name))
        throw Exception("Box already has a port with this name", name, this);
    ports[name] = port;
}

Port* Box::peekPort(QString name) {
    return ports.contains(name) ? ports.value(name) : 0;
}

Port* Box::seekPort(QString name) {
    Port *port = peekPort(name);
    if (!port)
        throw Exception("Box has no port with this name", name, this);
    return port;
}

void Box::initialize() {
    iteration = step = 0;
}

void Box::reset() {
    ++iteration;
    step = 0;
}

void Box::run() {
//    curRun = 0;
//    if (!amended) {
//        doAmend();
//        amended = true;
//    }
//    doInitialize();
//    for (; curRun < numRuns; ++curRun) {
//        curIteration = 0;
//        doReset();
//        for (; curIteration < numIterations; ++curIteration) {
//            doUpdate();
//        }
//        doCleanup();
//    }
//    doDebrief();
}

Box* Box::currentRoot() {
    if (!_currentRootIsDirty || _currentRoot==0)
        return _currentRoot;
    Box *p = _currentRoot;
    while (dynamic_cast<Box*>(p->parent()))
        p = dynamic_cast<Box*>(p->parent());
   return p;
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
RECURSE(initialize,doInitialize)
RECURSE(reset, doReset)
RECURSE(update, doUpdate)
RECURSE(cleanup, doCleanup)
RECURSE(debrief, doDebrief)

void Box::updateImports() {

}

}
