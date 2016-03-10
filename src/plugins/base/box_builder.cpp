#include <iostream>
#include "box.h"
#include "box_builder.h"
#include "box_step.h"
#include "mega_factory.h"
#include "path.h"

namespace base {

BoxBuilder::BoxBuilder()
    : _content(0), _currentBox(0), _currentPort(0)
{
}

BoxBuilder& BoxBuilder::box(QString className) {
    if (_content && _stack.isEmpty()) {
        throw Exception("BoxBuilder: Boxes miss a common root");
    }
    _stack.push(_currentBox);
    _currentBox =  MegaFactory::create<Box>(className, "", _currentBox);
    if (!_content)
        _content = _currentBox; // set _content on first call of box
    _currentPort = 0;
    return *this;
}

BoxBuilder& BoxBuilder::name(QString boxName) {
    _currentBox->setObjectName(boxName);
    return *this;
}

BoxBuilder& BoxBuilder::endbox() {
    if (_stack.isEmpty())
        throw Exception("BoxBuilder: box body ended twice");
    _currentBox = _stack.pop();
    _currentPort = 0;
    return *this;
}

BoxBuilder& BoxBuilder::port(QString name) {
    if (!_currentBox)
        throw Exception("BoxBuilder: port declaration outside of box context");
    _currentPort = _currentBox->peakPort(name) ?
                   _currentBox->port(name) :
                   new Port(name, _currentBox, true);
    return *this;
}

BoxBuilder& BoxBuilder::newPort(QString name) {
    if (!_currentBox) {
        throw Exception("BoxBuilder: new port declaration out of context");
    }
    _currentPort = new Port(name, _currentBox);
    _currentPort->access(Port::Read|Port::Write);
    return *this;
}

BoxBuilder& BoxBuilder::imports(QString pathToPort) {
    if (!_currentBox)
        throw Exception("BoxBuilder: import out of context");
    _currentPort->imports(pathToPort);
    return *this;
}

// Attributes by name

BoxBuilder& BoxBuilder::attribute(QString name, QString value) {
    if (!_currentPort)
        throw Exception("BoxBuilder: attribute out of context", name + "=" + value);
    _currentPort->attribute(name, value);
    return *this;
}

// Attributes direct

#define SET_ATTRIBUTE_DIRECT(X) \
BoxBuilder& BoxBuilder::X(QString value) { return attribute(#X, value); }

SET_ATTRIBUTE_DIRECT(format)
SET_ATTRIBUTE_DIRECT(page)
SET_ATTRIBUTE_DIRECT(plot)
SET_ATTRIBUTE_DIRECT(label)
SET_ATTRIBUTE_DIRECT(transform)

BoxBuilder& BoxBuilder::transform(PortTransform value) {
    return attribute("transform", convert<QString>(value));
}

// State

const Box* BoxBuilder::currentBox() const {
    return _currentBox;
}

const Port* BoxBuilder::currentPort() const {
    return _currentPort;
}

Box* BoxBuilder::content() {
    if (_content)
        _content->amendFamily();
    return _content;
}

}
