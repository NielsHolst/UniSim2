/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "box_builder.h"
#include "computation_step.h"
#include "dialog.h"
#include "environment.h"
#include "mega_factory.h"
#include "path.h"
#include "value.h"


namespace base {

BoxBuilder::BoxBuilder(Box *parent)
    : _hasParent(parent!=nullptr),
      _content(nullptr), _currentBox(nullptr), _currentPort(nullptr),
      _exceptionCount(Exception::count())
{
    if (_hasParent) {
        _content = _currentBox = parent;
        _stack.push(_currentBox);
    }
    else
        clear();
}

BoxBuilder::~BoxBuilder() {
    if (_content && _hasParent)
        content(BoxBuilder::Amend::Descendants, _exceptionCount==Exception::count());
}

void BoxBuilder::clear() {
    environment().computationStep(ComputationStep::Construct);
}

BoxBuilder& BoxBuilder::box(Box *box) {
    box->setParent(_currentBox);
    _currentPort = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::box(QString className) {
    if (_content && _stack.isEmpty())
        ThrowException("BoxBuilder: Boxes miss a common root");
    Box *newBox = MegaFactory::create<Box>(className, "", _currentBox);
    _stack.push(_currentBox);
    return moveToBox(newBox);
}

BoxBuilder& BoxBuilder::moveToBox(Box *box) {
    if (!box)
        ThrowException("BoxBuilder cannot move to non-existing box");
    _currentBox =  box;
    if (!_content)
        _content = _currentBox; // set _content on first call of box
    _currentPort = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::name(QString boxName) {
    _currentBox->setObjectName(boxName);
    return *this;
}

BoxBuilder& BoxBuilder::endbox() {
    if (_stack.isEmpty())
        ThrowException("BoxBuilder: box body ended twice");
    _currentBox = _stack.pop();
    _currentPort = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::port(QString name) {
    if (!_currentBox)
        ThrowException("BoxBuilder: port declaration outside of box context");
    _currentPort = _currentBox->port(name);
    return *this;
}

BoxBuilder& BoxBuilder::aux(QString name, QString type) {
    if (!_currentBox) {
        ThrowException("BoxBuilder: new port declaration out of context");
    }
    _currentPort = new Port(name, Port::Type::Auxiliary, _currentBox);
    if (!type.isEmpty())
        _currentPort->initialize(Value::create(type));
    // std::cerr << "BoxBuilder::aux " << qPrintable(name + " " + _currentPort->value().typeName())  << std::endl;
    return *this;
}


BoxBuilder& BoxBuilder::imports(QString pathToPort, Caller caller) {
    if (!_currentBox)
        ThrowException("BoxBuilder: import out of context");
    _currentPort->imports(pathToPort, caller);
    return *this;
}

// Set value

BoxBuilder& BoxBuilder::equals(const char *value) {
    return equals(QString(value));
}

BoxBuilder& BoxBuilder::equals(Expression expression) {
    if (!_currentPort)
        ThrowException("BoxBuilder: 'equals' must follow 'port'");
    // Assign the value to the port we are currently defining
    _currentPort->equals(expression);
    // std::cerr << "BoxBuilder::equals (expr)" << qPrintable(_currentPort->name() + " " + _currentPort->value().typeName())
              // << std::endl;
    return *this;
}

// State

const Box* BoxBuilder::currentBox() const {
    return _currentBox;
}

const Port* BoxBuilder::currentPort() const {
    return _currentPort;
}

Box* BoxBuilder::content(Amend amendOption, bool allowException) {
    if (_hasParent) {
        endbox();
        if (allowException && !_stack.isEmpty())
            ThrowException("BoxBuilder: unclosed box(es) at end")
                    .hint("Possibly missing endbox() call").value(_stack.size())
                    .context(_stack.top());
    }
    if (_content) {
        switch (amendOption) {
        case Amend::Family:
            _content->amendFamily();
            break;
        case Amend::Descendants:
            for (Box *child : _content->findMany<Box*>("./*"))
                child->amendFamily();
            break;
        case Amend::None:
            break;
        }

    }
    else
        ThrowException("Construction failed");
    // If we are returning a root box then enumerate it
    if (!_hasParent)
        Node::enumerate();
    return _content;
}

}
