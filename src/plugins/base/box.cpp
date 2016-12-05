#include <iostream>
#include "box.h"
#include "dialog.h"
#include "exception.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "timer.h"

namespace base {

Box *Box::_currentRoot = 0;
Box *Box::_savedCurrentRoot = 0;
int Box::_count = 0;

Box::Box(QString name, QObject *parent)
    : QObject(parent), _name(name), _order(0), _amended(false)
{
    Class(Box);
    setObjectName(name);
    _currentRoot = this;
    _timer = new Timer(this);
}

Box::~Box() {
    if (this == _currentRoot)
        _currentRoot = 0;
}

void Box::addPort(Port *port) {
    addPort(_ports, port);
}

void Box::addOrphanPort(Port *port) {
    port->setParent(0);
    addPort(_orphanPorts, port);
}

void Box::addPort(QMap<QString,Port*> &ports, Port *port) {
    QString name{port->objectName()};
    if (ports.contains(name))
        ThrowException("Box already has a port with this name").value(name).context(this);
    ports[name] = port;
}

#define PEAKPORT \
    Path path(".[" + name + "]", this); \
    auto ports = (path.resolve()); \
    QString value{" (%1 found)"}; \
    int n = ports.size(); \
    switch (n) { \
    case 0: \
        return 0; \
    case 1: \
        return dynamic_cast<Port*>(ports.at(0)); \
    default: \
        ThrowException("Port name not unique within box").value(name+value.arg(n)).context(this);; \
    }

Port* Box::peakPort(QString name) {
    return _ports.contains(name) ? _ports.value(name) : 0;
}

const Port* Box::peakPort(QString name) const {
    return _ports.contains(name) ? _ports.value(name) : 0;
}

Port* Box::port(QString name) {
    Port *port = peakPort(name); \
    if (!port) \
        ThrowException("No port of that name in this box").value(name).context(this); \
    return port; \
}

const Port* Box::port(QString name) const {
    const Port *port = peakPort(name); \
    if (!port) \
        ThrowException("No port of that name in this box").value(name).context(this); \
    return port; \
}

void Box::help(QString s) {
    _help = s;
}

QString Box::help() const {
    return _help;
}

void Box::sideEffects(QString s) {
    _sideEffects = s;
}

QString Box::sideEffects() const {
    return _sideEffects;
}

Box* Box::currentRoot() {
    if (_currentRoot==0)
        return 0;
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

QString Box::className() const {
    return base::className(this);
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

void Box::postAmend() {
    // Copy attributes from orphan ports to original ports
    QMapIterator<QString, Port*> orphanIt(_orphanPorts);
    while (orphanIt.hasNext()) {
        orphanIt.next();
        QString orphanName = orphanIt.key();
        Port *orphanPort = orphanIt.value(),
             *originalPort = peakPort(orphanName);
        if (!originalPort)
            ThrowException("Port not found").value(orphanName).context(this);
        for (QString name : orphanPort->attributes()) {
            // Only copy attribute if it has been set in the orphan port
            bool attributeHasBeenSet = !orphanPort->attribute(name).isNull();
            if (attributeHasBeenSet)
                originalPort->attribute(name, orphanPort->attribute(name));
        }
    }
    // Clear orphan list
    for (Port *orphan : _orphanPorts)
        delete orphan;
    _orphanPorts.clear();
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
        amend();
        postAmend();
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
    _timer->addProfile("amend");
    _timer->addProfile("initialize");
    _timer->addProfile("reset");
    _timer->addProfile("update-updateImports");
    _timer->addProfile("update-update");
    _timer->addProfile("update-trackPorts");
    _timer->addProfile("cleanup");
    _timer->addProfile("debrief");
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
    _timer->reset();
    if (!_amended)
        amendFamily();
    _timer->start("initialize");
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->initializeFamily();
    }
    resolvePortImports();
    allocatePortBuffers();
    collectTrackedPorts();
    updateImports();
    initialize();
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
    reset();
    trackPorts(Reset);
    _timer->stop("reset");
}

void Box::updateFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->updateFamily();
    }

    _timer->start("update-updateImports");
    updateImports();
    _timer->stop("update-updateImports");

    _timer->start("update-update");
    update();
    _timer->stop("update-update");

    _timer->start("update-trackPorts");
    trackPorts(Update);
    _timer->stop("update-trackPorts");
}

void Box::cleanupFamily() {
    _timer->start("cleanup");
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->cleanupFamily();
    }
    updateImports();
    cleanup();
    trackPorts(Cleanup);
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
    debrief();
    _timer->stop("debrief");
}

void Box::resolvePortImports() {
    for (Port *port : _ports.values()) {
        port->resolveImports();
        if (!port->warnings().isEmpty())
            dialog().error("Warnings:\n" + port->warnings().join("\n"));
    }
}

void Box::allocatePortBuffers(){
    for (Port *port : _ports.values())
        port->allocatePortBuffer();
}

void Box::collectTrackedPorts() {
    _trackedPorts.clear();
    for (Port *port : _ports.values()) {
        if (port->hasTrack())
            _trackedPorts << port;
    }
}

void Box::updateImports() {
    for (Port *port : _ports.values())
        port->copyFromImport();
}

void Box::resetPorts() {
    for (Port *port : _ports.values())
        port->reset();
}

void Box::trackPorts(Step step) {
    for (Port *port : _trackedPorts)
        port->track(step);
}


}
