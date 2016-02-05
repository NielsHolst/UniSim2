#include <iostream>
#include "box.h"
#include "box_builder.h"
#include "box_step.h"
#include "mega_factory.h"

namespace base {

BoxBuilder::BoxBuilder()
    : _content(0), _currentBox(0), _currentPort(0)
{
}

BoxBuilder& BoxBuilder::box(QString className) {
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

BoxBuilder& BoxBuilder::newPort(QString name) {
    if (!_currentBox) {
        throw Exception("BoxBuilder: 'newPort' must be between 'box' and 'endbox'");
    }
    _currentPort = new Port(name, _currentBox);
    _currentPort->access(Port::Read|Port::Write);
    _currentPort->trackOff();
    return *this;
}

BoxBuilder& BoxBuilder::port(QString name) {
    if (!_currentBox) {
        throw Exception("BoxBuilder: 'port' must be between 'box' and 'endbox'");
    }
    _currentPort = _currentBox->port(name);
    return *this;
}

BoxBuilder& BoxBuilder::import(QString pathToPort) {
    if (!_currentBox)
        throw Exception("BoxBuilder: 'port' missing before 'import'");
    _currentPort->import(pathToPort);
    return *this;
}

BoxBuilder& BoxBuilder::trackOn() {
    if (!_currentPort)
        throw Exception("BoxBuilder: 'port' missing before 'trackOn'");
    _currentPort->trackOn();
    return *this;
}

BoxBuilder& BoxBuilder::trackOff() {
    if (!_currentPort)
        throw Exception("BoxBuilder: 'port' missing before 'trackOff'");
    _currentPort->trackOff();
    return *this;
}

BoxBuilder& BoxBuilder::transform(PortTransform pt) {
    if (!_currentPort)
        throw Exception("BoxBuilder: 'port' missing before 'transform'");
    _currentPort->transform(pt);
    return *this;
}

BoxBuilder& BoxBuilder::endbox() {
    if (_stack.isEmpty())
        throw Exception("BoxBuilder: 'endbox' without matching 'box'");
    _currentBox = _stack.pop();
    _currentPort = 0;
    return *this;
}

Box* BoxBuilder::content() {
    return _content;
}

}
