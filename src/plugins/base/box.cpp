/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include "box.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "general.h"
#include "mega_factory.h"
#include "path.h"
#include "port.h"
#include "timer.h"

namespace base {

Box *Box::_currentRoot = nullptr;
Box *Box::_savedCurrentRoot = nullptr;
int Box::_count = 0;
bool Box::_debugOn = false;
bool Box::_traceOn = false;

Box::Box(QString name, QObject *parent, Persistence persistence)
    : QObject(parent), _order(0), _amended(false), _cloned(false), _ignore(false)
{
    Class(Box);
    help("has no documented functionality");
    setObjectName(name);
    _currentRoot = this;
    _timer = new Timer(this);
    if (persistence == AllowRemoval)
        Input(remove).equals(false).help("Remove this box?");
    else
        remove = false;
}

Box::~Box() {
    if (this == _currentRoot)
        _currentRoot = nullptr;
}

void Box::addPort(Port *port) {
    addPort(_ports, port);
}

void Box::addPort(QMap<QString,Port*> &ports, Port *port) {
    QString name{port->objectName()};
    if (ports.contains(name))
        ThrowException("Box already has a port with this name").value(name).context(this);
    ports[name] = port;
}

Port* Box::peakPort(QString name) {
    return _ports.contains(name) ? _ports.value(name) : nullptr;
}

const Port* Box::peakPort(QString name) const {
    return _ports.contains(name) ? _ports.value(name) : nullptr;
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

void Box::removeChild(QString name) {
    Box *child = findOne<Box>(name);
    dialog().information("Removing box " + child->fullName());
    delete child;
}

Box* Box::currentRoot() {
    if (_currentRoot==nullptr)
        return nullptr;
    while (true) {
        Box *p = dynamic_cast<Box*>(_currentRoot->parent());
        if (!p) break;
        _currentRoot = p;
    }
    return _currentRoot;
}

void Box::saveCurrentRoot() {
    _savedCurrentRoot = _currentRoot;
}

void Box::restoreCurrentRoot() {
    _currentRoot = _savedCurrentRoot;
}

Box* Box::boxParent() {
    return dynamic_cast<Box*>(parent());
}

QString Box::className() const {
    return base::className(this);
}

QString Box::name() const {
    return objectName();
}

QString Box::fullName() const {
    return base::fullName(this);
}

int Box::order() const {
    return _order;
}

int Box::count() {
    return _count;
}

QString Box::profileReport() const {
    QString rep = _timer->report();
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            rep += box->profileReport();
    }
    return rep;
}

void Box::run() {
    ThrowException("Method 'run' not defined for this class").value(className()).context(this);
}

void Box::amendFamily() {
    try {
        if (_amended) return;
        createTimers();
        _timer->start("amend");
        for (auto child : children()) {
            Box *box = dynamic_cast<Box*>(child);
            if (box)
                box->amendFamily();
        }
        if (_traceOn)
            dialog().information("amend " + fullName());
        if (!_ignore) amend();
        _count = 0;
        enumerateBoxes(_count);
        _amended = true;
        _timer->stop("amend");
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
    _timer->addProfile(name);
}

void Box::startTimer(QString name) {
    _timer->start(name);
}

void Box::stopTimer(QString name) {
    _timer->stop(name);
}

void Box::enumerateBoxes(int &i) {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->enumerateBoxes(i);
    }
    _order = i++;
}

void Box::initializeFamily() {
    if (_traceOn)
        dialog().information("initializeFamily " + fullName());
    if (!_amended) {
        environment().computationStep(ComputationStep::Amend, false);
        amendFamily();
        environment().computationStep(ComputationStep::Initialize, false);
    }
    _timer->reset();
    _timer->start("initialize");
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->initializeFamily();
    }
    closeExpressions();
    updateImports();
    checkSelfImports();
    if (_traceOn)
        dialog().information("initialize " + fullName());
    if (!_ignore) initialize();
    _timer->stop("initialize");
}

void Box::resetFamily() {
    _timer->start("reset");
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->resetFamily();
    }
    resetPorts();
    updateImports();
    if (_traceOn)
        dialog().information("reset " + fullName());
    if (!_ignore) reset();
    verifyPorts();
    _timer->stop("reset");
}

void Box::updateFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->updateFamily();
    }
    updateImports();
    if (_traceOn)
        dialog().information("update " + fullName());

    _timer->start("update");
    if (!_ignore) update();
    _timer->stop("update");

    verifyPorts();

}

void Box::cleanupFamily() {
    _timer->start("cleanup");
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->cleanupFamily();
    }
    updateImports();
    if (_traceOn)
        dialog().information("cleanup " + fullName());
    if (!_ignore) cleanup();
    verifyPorts();
    _timer->stop("cleanup");
}

void Box::debriefFamily() {
    _timer->start("debrief");
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->debriefFamily();
    }
    updateImports();
    if (_traceOn)
        dialog().information("debrief " + fullName());
    if (!_ignore) debrief();
    verifyPorts();
    _timer->stop("debrief");
}

void Box::closeExpressions() {
    for (Port *port : _ports.values()) {
        port->closeExpression();
    }
}

void Box::verifyPorts() {
    if (_debugOn) {
        for (Port *port : _ports.values())
            port->verifyValue();
    }
}

void Box::resetPorts() {
    for (Port *port : _ports.values())
        port->reset();
}

Box* Box::clone(QString name, QObject *parent) {
    Box *myClone = MegaFactory::create<Box>(className(), name, parent);
    myClone->_cloned = true;
    for (Port *port : findMany<Port>(".[*]")) {
        QString name = port->objectName();
        // For a blind port, or a port which the clone hasn't got (because it was created in the original's amend):
        // create it in the clone
        if (port->isBlind()) {
            Port *blind = new Port(name, myClone);
            blind->access(PortAccess::Input).isBlind(true);
        }
        else if (!myClone->peakPort(name)) {
            Port *newPort = new Port(name, myClone);
            newPort->access(port->access());
        }
        // Initialize to same type and value
        myClone->port(name)->equals( port->value<Value>() );
    }
    return myClone;
}

Box* Box::cloneFamily(QString name, QObject *parent) {
    Box *myClone = clone(name, parent);
    for (Box *child : findMany<Box>("./*"))
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
        options = "io";
    bool writeOverriddenInputs = options.contains("I"),
         writeAllInputs = options.contains("i") && !writeOverriddenInputs,
         writeOutputs = options.contains("o");

    Box *me = const_cast<Box*>(this);
    QString fill;
    fill.fill(' ', indentation);

    QString postfix = (constructionStep() == ComputationStep::Amend) ? " //amended" : "";

    text << fill << className() << " " << objectName()
         << " {" << postfix
//         << " // #" << order()
         << "\n";

    for (Port *port : me->findMany<Port>(".[*]")) {
        bool isInput = port->access() == PortAccess::Input,
             isOverridden = port->isValueOverridden(),
             doWrite = isInput &
                       (writeAllInputs || (writeOverriddenInputs && isOverridden));
        if (doWrite)
            port->toText(text, indentation+2);
    }

    for (Port *port : me->findMany<Port>(".[*]")) {
        bool isOutput = port->access() == PortAccess::Output,
             doWrite = writeOutputs && isOutput;
        if (doWrite)
            port->toText(text, indentation+2);
    }

    for (Box *box : me->findMany<Box>("./*")) {
        box->toText(text, options, indentation+2);
    }
    text << fill << "}\n";
}

}
