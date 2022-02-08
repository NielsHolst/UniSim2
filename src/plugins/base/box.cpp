/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include "box.h"
#include "computation.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "mega_factory.h"
#include "path.h"
#include "port.h"
#include "timer.h"

namespace base {

Box *Box::_latest = nullptr;
bool Box::_debugOn = false;
bool Box::_traceOn = false;

Box::Box(QString name, Box *parent)
    : Node(name, parent), _amended(false), _cloned(false), _timer(this)
{
    help("has no documented functionality");
    if (!_latest || parent)
        _latest = this;
}

Box::~Box() {
    if (this == _latest)
        _latest = nullptr;
}

void Box::addPort(Port *port) {
    addPort(_portMap, port);
    port->setOrder(_portsInOrder.size());
    _portsInOrder << port;
}

void Box::addPort(QMap<QString,Port*> &ports, Port *port) {
    QString name{port->objectName()};
    if (ports.contains(name))
        ThrowException("Box already has a port with this name").value(name).context(this);
    ports[name] = port;

}

Port* Box::peakPort(QString name) {
    return _portMap.contains(name) ? _portMap.value(name) : nullptr;
}

const Port* Box::peakPort(QString name) const {
    return _portMap.contains(name) ? _portMap.value(name) : nullptr;
}

Port* Box::port(QString name, Caller caller) {
    Port *port = peakPort(name);
    if (!port)
        ThrowException("No port of that name in this box").value(name).context(this).caller(caller);
    return port;
}

const Port* Box::port(QString name, Caller caller) const {
    const Port *port = peakPort(name);
    if (!port) {
        ThrowException("No port of that name in this box").value(name).context(this).caller(caller);
    }
    return port;
}

void Box::help(QString s) {
    _help = s;
}

QString Box::help() const {
    // Use help text only in derived classes
    return (className()=="Box") ? "is just as a container box" : _help;
}

void Box::sideEffects(QString s) {
    _sideEffects = s;
}

QString Box::sideEffects() const {
    return _sideEffects;
}

Box *Box::findRoot() {
    Box *p = parent<Box*>();
    return p ? p->findRoot() : this;
}

Box *Box::root() {
    return _latest ? _latest->findRoot() : nullptr;
}

QString Box::profileReport() {
    QString rep = _timer.report();
    for (auto box : children<Box*>()) {
        rep += box->profileReport();
    }
    return rep;
}

void Box::run() {
    ThrowException("Method 'run' not defined for this class").value(className()).context(this);
}

void Box::amendFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Amend);
    try {
        if (_amended) return;
        createTimers();
        _timer.start("amend");
        for (auto box : children<Box*>())
            box->amendFamily(false);
        if (_traceOn) trace("amend");
        amend();
        touchPorts();
        _amended = true;
        _timer.stop("amend");
    }
    catch (Exception &ex) {
        dialog().error(ex.what());
    }
}

void Box::createTimers() {
    createTimer("amend");
    createTimer("initialize");
    createTimer("reset");
    createTimer("update");
    createTimer("updateImports");
    createTimer("updateSelfImports");
    createTimer("cleanup");
    createTimer("debrief");
}

void Box::createTimer(QString name) {
    _timer.addProfile(name);
}

void Box::startTimer(QString name) {
    _timer.start(name);
}

void Box::stopTimer(QString name) {
    _timer.stop(name);
}

void Box::initializeFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Initialize);
    _timer.reset();
    _timer.start("initialize");
    for (auto box : children<Box*>())
        box->initializeFamily(false);
    if (_traceOn) trace("initialize");
    evaluatePorts();
    initialize();
    _timer.stop("initialize");
}

void Box::resetFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Reset);
    _timer.start("reset");
    for (auto box : children<Box*>())
        box->resetFamily(false);
    if (_traceOn) trace("reset");
    clearPorts();
    evaluatePorts();
    reset();
    verifyPorts();
    _timer.stop("reset");
}

void Box::updateFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Update);
    _timer.start("update");
    for (auto box : children<Box*>())
        box->updateFamily(false);
    if (_traceOn) trace("update");
    evaluatePorts();
    update();
    verifyPorts();
    _timer.stop("update");
}

void Box::cleanupFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Cleanup);
    _timer.start("cleanup");
    for (auto box : children<Box*>())
        box->cleanupFamily(false);
    if (_traceOn) trace("cleanup");
    evaluatePorts();
    cleanup();
    verifyPorts();
    _timer.stop("cleanup");
}

void Box::debriefFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Debrief);
    _timer.start("debrief");
    for (auto box : children<Box*>())
        box->debriefFamily(false);
    if (_traceOn) trace("debrief");
    evaluatePorts();
    debrief();
    verifyPorts();
    _timer.stop("debrief");
    if (announce) Computation::changeStep(Computation::Step::Ready);
}

const QVector<Port*> &Box::portsInOrder() {
    return _portsInOrder;
}

void Box::clearPorts() {
    for (Port *port : _portsInOrder)
        port->clear();
}

void Box::touchPorts() {
    for (Port *port : _portsInOrder)
        port->touch();
}

void Box::evaluatePorts() {
    for (Port *port : _portsInOrder)
        port->evaluate();
}

void Box::verifyPorts() {
    if (_debugOn) {
        for (Port *port : _portsInOrder)
            port->verifyValue();
    }
}

Box* Box::clone(QString name, Box *parent) {
    Box *myClone = MegaFactory::create<Box>(className(), name, parent);
    myClone->_cloned = true;
    // Loop through my ports
    for (Port *port : findMany<Port*>(".[*]")) {
        // Find correspondin port in clone
        QString name = port->objectName();
        Port *clonedPort = myClone->peakPort(name);
        // Create port in clone, if not found
        if (!clonedPort)
            clonedPort = new Port(name, port->type(), myClone);
        // Copy my port to clone
        *clonedPort = *port;
    }
    return myClone;
}

Box* Box::cloneFamily(QString name, Box *parent) {
    Box *myClone = clone(name, parent);
    for (Box *child : findMany<Box*>("./*"))
        child->cloneFamily(child->objectName(), myClone);
    return myClone;
}

bool Box::cloned() const {
    return _cloned;
}

void Box::debug(bool on) {
    _traceOn = _debugOn = on;
}

bool Box::debug() {
    return _debugOn;
}

void Box::toText(QTextStream &text, QString options, int indentation) const {
    if (options.isEmpty())
        options = "ioa";
    bool writeOverriddenInputs = options.contains("I"),
         writeAllInputs        = options.contains("i") && !writeOverriddenInputs,
         writeOutputs          = options.contains("o"),
         writeAux              = options.contains("a");

    QString fill;
    fill.fill(' ', indentation);

    text << fill << className() << " " << objectName() << " { //" << order() << "\n";

    for (Port *port : _portsInOrder) {
        bool doWrite;
        switch (port->type()) {
        case PortType::Input:
            doWrite = writeAllInputs || (writeOverriddenInputs && port->isValueOverridden());
            break;
        case PortType::Output:
            doWrite = writeOutputs;
            break;
        case PortType::Auxiliary:
            doWrite = writeAux;
            break;
        }
        if (doWrite)
            port->toText(text, indentation+2);
    }

    Box *me = const_cast<Box*>(this);
    for (Box *box : me->findMany<Box*>("./*"))
        box->toText(text, options, indentation+2);

    text << fill << "}\n";
}

QString Box::toText(QString options) const {
    QString s;
    QTextStream text(&s);
    toText(text, options, 0);
    return s;
}

void Box::trace(QString id) const {
    dialog().information(id + " " + fullName());
}

}
