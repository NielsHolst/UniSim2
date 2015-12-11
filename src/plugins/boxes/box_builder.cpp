#include <iostream>
#include "box.h"
#include "box_builder.h"

namespace boxes {

BoxBuilder::BoxBuilder()
    : _content(0), _currentBox(0), _currentPort(0)
{
}

BoxBuilder& BoxBuilder::box(QString name) {
    _stack.push(_currentBox);
    _currentBox = new Box(name, _currentBox);
    if (!_content)
        _content = _currentBox; // set _content on first call of box
    _currentPort = 0;
    return *this;
}

BoxBuilder& BoxBuilder::port(QString name) {
    if (!_currentBox) {
        throw Exception("BoxBuilder:'port' must be between 'box' and 'endbox'");
    }
    _currentPort = new Port(name, _currentBox);
    return *this;
}

BoxBuilder& BoxBuilder::endbox() {
    if (_stack.isEmpty())
        throw Exception("BoxBuilder:'endbox' without matching 'box'");
    _currentBox = _stack.pop();
    _currentPort = 0;
    return *this;
}

Box* BoxBuilder::content() {
    return _content;
}

}
