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

std::unique_ptr<Box> Box::_root;
Box *Box::_latest = nullptr;
bool Box::_debugOn = false;
bool Box::_traceOn = false;

Box::Box(QString name, Box *parent)
    : Node(name, parent), _amended(false), _initialized(false), _cloned(false), _timer(this)
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

bool Box::hasRoot() {
    return _root.get();
}

Box* Box::root(Box *box) {
    if (!box)
        _root.reset();
    else {
        auto newRoot = std::unique_ptr<Box>(box);
        _root = std::move(newRoot);
        environment().current(_root.get());
    }
    return _root.get();
}

Box* Box::root() {
    if (Computation::currentStep() <= Computation::Step::Amend ||
        Computation::currentStep() == Computation::Step::Scratch)
        return _latest ? _latest->latestRoot() : nullptr;
    else if (!_root)
        ThrowException("Missing root box");
    return _root.get();
}

Box *Box::latestRoot() {
    Box *p = parent<Box*>();
    return p ? p->latestRoot() : this;
}

QString Box::profileReport() {
    QString result;
    auto report = collectTimerReport();
    std::sort(report.begin(), report.end(),
              [](Timer::ReportItem a, Timer::ReportItem b) { return a.duration > b.duration; });
    for (auto item : report) {
        result +=
                item.parent->fullName() + "\t" +
                item.name + "\t" +
                QString::number(item.duration) + "\n";
    }
    return result;
}

Timer::Report Box::collectTimerReport() {
    Timer::Report result = _timer.report();
    for (auto box : children<Box*>())
        result.append(box->collectTimerReport());
    return result;
}

void Box::run() {
    ThrowException("Method 'run' not defined for this class").value(className()).context(this);
}

void Box::amendFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Amend);
    if (_amended)
        ThrowException("Box already amended").context(this);
    _amended = true;
    createTimers();
    _timer.start("amend");
    for (auto box : children<Box*>())
        box->amendFamily(false);
    if (_traceOn) trace("amend");
    amend();
    touchPorts();
    _timer.stop("amend");
}

void Box::createTimers() {
    createTimer("amend");
    createTimer("initialize");
    createTimer("reset");
    createTimer("update");
    createTimer("cleanup");
    createTimer("debrief");
    createTimer("evaluatePorts");
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
    if (!_amended)
        ThrowException("Box must be amended before initialisation").context(this);
    if (_initialized)
        ThrowException("Box already initialized").context(this);
    _initialized = true;

    _timer.reset();
    for (auto box : children<Box*>())
        box->initializeFamily(false);
    if (_traceOn) trace("initialize");
    evaluatePorts();
    _timer.start("initialize");
    initialize();
    _timer.stop("initialize");
}

void Box::resetFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Reset);
    for (auto box : children<Box*>())
        box->resetFamily(false);
    if (_traceOn) trace("reset");
    clearPorts();
    evaluatePorts();
    _timer.start("reset");
    reset();
    verifyPorts();
    _timer.stop("reset");
}

void Box::updateFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Update);
    for (auto box : children<Box*>())
        box->updateFamily(false);
    if (_traceOn) trace("update");
    evaluatePorts();
    _timer.start("update");
    update();
    verifyPorts();
    _timer.stop("update");
}

void Box::cleanupFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Cleanup);
    for (auto box : children<Box*>())
        box->cleanupFamily(false);
    if (_traceOn) trace("cleanup");
    evaluatePorts();
    _timer.start("cleanup");
    cleanup();
    verifyPorts();
    _timer.stop("cleanup");
}

void Box::debriefFamily(bool announce) {
    if (announce) Computation::changeStep(Computation::Step::Debrief);
    for (auto box : children<Box*>())
        box->debriefFamily(false);
    if (_traceOn) trace("debrief");
    evaluatePorts();
    _timer.start("debrief");
    debrief();
    verifyPorts();
    _timer.stop("debrief");
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
    _timer.start("evaluatePorts");
    for (Port *port : _portsInOrder)
        port->evaluate();
    _timer.stop("evaluatePorts");
}

void Box::verifyPorts() {
    if (_debugOn) {
        for (Port *port : _portsInOrder)
            port->verifyValue();
    }
}

void Box::registerPorts() {
    for (Box *box : children<Box*>())
        box->registerPorts();
    for (Port *port : _portsInOrder)
        port->registerExports();
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
