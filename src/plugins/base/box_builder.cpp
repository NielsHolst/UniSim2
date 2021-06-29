/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QObject>
#include "box.h"
#include "box_builder.h"
#include "computation_step.h"
#include "dialog.h"
#include "distribution.h"
#include "environment.h"
#include "mega_factory.h"
#include "path.h"
#include "port_filter.h"
#include "track.h"

namespace base {

BoxBuilder::BoxBuilder(Box *parent)
    : _hasParent(parent!=nullptr),
      _content(nullptr), _currentBox(nullptr), _currentPort(nullptr), _currentDistribution(nullptr),
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
        content(BoxBuilder::AmendDescendants, _exceptionCount==Exception::count());
}

void BoxBuilder::clear() {
    environment().computationStep(ComputationStep::Construct);
    Track::clearOrders();
    Port::clearIndex();
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
    _currentDistribution = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::port(QString name) {
    if (!_currentBox)

        ThrowException("BoxBuilder: port declaration outside of box context");
    _currentPort = _currentBox->port(name);
    _currentDistribution = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::newPort(QString name) {
    if (!_currentBox) {
        ThrowException("BoxBuilder: new port declaration out of context");
    }
    _currentPort = new Port(name, _currentBox);
    _currentPort->isBlind(true);
    _currentDistribution = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::rnd(QString value) {
    if (!_currentPort)
        ThrowException("BoxBuilder: rnd declaration out of context");
    value = value.simplified();
    #if QT_VERSION >= 0x050E00
        QStringList items = value.split(" ", Qt::SkipEmptyParts);
    #else
        QStringList items = value.split(" ", QString::SkipEmptyParts);
    #endif
    if (items.empty())
        ThrowException("Missing distribution parameters").context(_currentPort);

    QString className = items.first();
    items.removeAt(0);

    _currentDistribution = MegaFactory::create<Distribution>(className, className, _currentPort);
    _currentDistribution->arguments(items);
    _currentDistribution->port(_currentPort);
    return *this;
}

#define BUILD_DISTRIBUTION(X) \
BoxBuilder& BoxBuilder::X(double value) { \
    _currentDistribution->X(value); \
    return *this; \
}
BUILD_DISTRIBUTION(mean)
BUILD_DISTRIBUTION(sd)
BUILD_DISTRIBUTION(min)
BUILD_DISTRIBUTION(max)
BUILD_DISTRIBUTION(lowerQuantile)
BUILD_DISTRIBUTION(upperQuantile)

BoxBuilder& BoxBuilder::imports(QString pathToPort, Caller caller) {
    if (!_currentBox)
        ThrowException("BoxBuilder: import out of context");
    _currentPort->imports(pathToPort, caller);
    _currentDistribution = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::importsMaybe(QString pathToPort, QString fallBackValue, Caller caller) {
    if (!_currentBox)
        ThrowException("BoxBuilder: import out of context");
    _currentPort->importsMaybe(pathToPort, fallBackValue, caller);
    _currentDistribution = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::track(PortFilter filter) {
    _currentPort->track(filter);
    return *this;
}

// Attributes by name

BoxBuilder& BoxBuilder::attribute(QString name, QString value) {
    if (!_currentPort)
        ThrowException("BoxBuilder: attribute out of context").value(name + "=" + value);
    _currentPort->attribute(name, value);
    return *this;
}

// Attributes direct

#define SET_ATTRIBUTE_DIRECT(X) \
BoxBuilder& BoxBuilder::X(QString value) { return attribute(#X, value); }

SET_ATTRIBUTE_DIRECT(format)
//SET_ATTRIBUTE_DIRECT(label)
SET_ATTRIBUTE_DIRECT(transform)

BoxBuilder& BoxBuilder::transform(PortTransform value) {
    return attribute("transform", convert<QString>(value));
}

// Set value

BoxBuilder& BoxBuilder::equals(const char *value, bool ignore) {
    return equals(QString(value), ignore);
}

// State

const Box* BoxBuilder::currentBox() const {
    return _currentBox;
}

const Port* BoxBuilder::currentPort() const {
    return _currentPort;
}

Box* BoxBuilder::content(AmendOption amendOption, bool allowException) {
    if (_hasParent) {
        endbox();
        if (allowException && !_stack.isEmpty())
            ThrowException("BoxBuilder: unclosed box(es) at end")
                    .hint("Possibly missing endbox() call").value(_stack.size())
                    .context(_stack.top());
    }
    if (_content) {
        switch (amendOption) {
        case AmendFamily:
            _content->amendFamily();
            break;
        case AmendDescendants:
            for (Box *child : _content->findMany<Box>("./*"))
                child->amendFamily();
            break;
        case AmendNone:
            break;
        }

    }
    else
        ThrowException("Construction failed");
    return _content;
}

}
