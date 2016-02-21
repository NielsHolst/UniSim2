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
    if (!_currentBox) {
        throw Exception("BoxBuilder: port declaration outside of box context");
    }
    _currentPort = _currentBox->port(name);
    return *this;
}

BoxBuilder& BoxBuilder::newPort(QString name) {
    if (!_currentBox) {
        throw Exception("BoxBuilder: new port declaration out of context");
    }
    _currentPort = new Port(name, _currentBox);
    _currentPort->access(Port::Read|Port::Write);
    _currentPort->trackOff();
    return *this;
}

BoxBuilder& BoxBuilder::imports(QString pathToPort) {
    if (!_currentBox)
        throw Exception("BoxBuilder: import out of context");
    _currentPort->imports(pathToPort);
    return *this;
}

BoxBuilder& BoxBuilder::attribute(QString name, QString value) {
    if (name == "axis") return axis(value);
    if (name == "plot") return plot(value);
    if (name == "label") return label(value);
    if (name == "page") return page(value);
    if (name == "Rformat") return Rformat(value);
    if (name == "track") return track(value);
    throw Exception("Unknown port attribute", name);
}

BoxBuilder& BoxBuilder::axis(QString ax) {
    if (!_currentPort)
        throw Exception("BoxBuilder: axis out of context");
    _currentPort->axis(ax);
    return *this;
}

BoxBuilder& BoxBuilder::plot(QString pl) {
    if (!_currentPort)
        throw Exception("BoxBuilder: plot out of context");
    _currentPort->plot(pl);
    return *this;
}

BoxBuilder& BoxBuilder::label(QString la) {
    if (!_currentPort)
        throw Exception("BoxBuilder: label out of context");
    _currentPort->label(la);
    return *this;
}

BoxBuilder& BoxBuilder::page(QString pa) {
    if (!_currentPort)
        throw Exception("BoxBuilder: page out of context");
    _currentPort->page(pa);
    return *this;
}

BoxBuilder& BoxBuilder::track(QString tr) {
    if (tr == "on")
        return trackOn();
    else if (tr == "off")
        return trackOff();
    else
        throw Exception("Track value must be 'on' or 'off'", tr);
}

BoxBuilder& BoxBuilder::trackOn() {
    if (!_currentPort)
        throw Exception("BoxBuilder: track out of context");
    _currentPort->trackOn();
    return *this;
}

BoxBuilder& BoxBuilder::trackOff() {
    if (!_currentPort)
        throw Exception("BoxBuilder: track out of context");
    _currentPort->trackOff();
    return *this;
}

BoxBuilder& BoxBuilder::Rformat(QString format) {
    if (!_currentPort)
        throw Exception("BoxBuilder: Rformat out of context");
    _currentPort->Rformat(format);
    return *this;
}

BoxBuilder& BoxBuilder::transform(PortTransform pt) {
    if (!_currentPort)
        throw Exception("BoxBuilder: transform out of context");
    _currentPort->transform(pt);
    return *this;
}

const Box* BoxBuilder::currentBox() const {
    return _currentBox;
}

const Port* BoxBuilder::currentPort() const {
    return _currentPort;
}

Box* BoxBuilder::content() {
    return _content;
}

}
