#include <iostream>
#include "box.h"
#include "exception.h"
#include "general.h"
#include "path.h"
#include "port.h"

namespace base {

Box *Box::_currentRoot = 0;
bool Box::_currentRootIsDirty = true;
int Box::_count = 0;

Box::Box(QString name, QObject *parent)
    : QObject(parent), _name(name), _id(0), _amended(false)
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
    addPort(_ports, port);
}

void Box::addOrphanPort(Port *port) {
    port->setParent(0);
    addPort(_orphanPorts, port);
}

void Box::addPort(QMap<QString,Port*> &ports, Port *port) {
    QString name{port->objectName()};
    if (ports.contains(name))
        throw Exception("Box already has a port with this name", name, this);
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
        throw Exception("Port name not unique within box", name+value.arg(n), this); \
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
        throw Exception("No port of that name in this box", name, this); \
    return port; \
}

const Port* Box::port(QString name) const {
    const Port *port = peakPort(name); \
    if (!port) \
        throw Exception("No port of that name in this box", name, this); \
    return port; \
}

Box* Box::currentRoot() {
    if (!_currentRootIsDirty || _currentRoot==0)
        return _currentRoot;
    Box *p = _currentRoot;
    while (dynamic_cast<Box*>(p->parent()))
        p = dynamic_cast<Box*>(p->parent());
   return p;
}

QString Box::className() const {
    return base::className(this);
}

QString Box::fullName() const {
    return base::fullName(this);
}

int Box::id() const {
    return _id;
}

int Box::count() {
    return _count;
}

void Box::run() {
    throw Exception("Method 'run' not defined for this class", className(), this);
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
            throw Exception("Port not found", orphanName, this);
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
    if (_amended) return;
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
}

void Box::enumerateBoxes(int &i) {
    _id = i++;
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->enumerateBoxes(i);
    }
}

void Box::initializeFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->initializeFamily();
    }
    resolvePortImports();
    allocatePortBuffers();
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
    trackPorts(Reset);
}

void Box::updateFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->updateFamily();
    }
    updateImports();
    update();
    trackPorts(Update);
}

void Box::cleanupFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->cleanupFamily();
    }
    updateImports();
    cleanup();
    trackPorts(Cleanup);
}

void Box::debriefFamily() {
    for (auto child : children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            box->debriefFamily();
    }
    updateImports();
    debrief();
}

void Box::resolvePortImports() {
    for (Port *port : _ports.values())
        port->resolveImports();
}

void Box::allocatePortBuffers(){
    for (Port *port : _ports.values())
        port->allocatePortBuffer();
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
    for (Port *port : _ports.values())
        port->track(step);
}


}
