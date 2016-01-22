#include <iostream>
#include "box.h"
#include "box_builder.h"
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

BoxBuilder& BoxBuilder::transform(PortTransform pt) {
    if (!_currentBox)
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
