#include <iostream>
#include "box.h"
#include "box_builder.h"
#include "box_step.h"
#include "computation_step.h"
#include "dialog.h"
#include "distribution.h"
#include "environment.h"
#include "mega_factory.h"
#include "path.h"

namespace base {

BoxBuilder::BoxBuilder()
    : _content(0), _currentBox(0), _currentPort(0), _currentDistribution(0)
{
    environment().computationStep(ComputationStep::Construct);
}

BoxBuilder& BoxBuilder::box(Box *box) {
    box->setParent(_currentBox);
    _currentPort = 0;
    return *this;
}

BoxBuilder& BoxBuilder::box(QString className) {
    if (_content && _stack.isEmpty()) {
        ThrowException("BoxBuilder: Boxes miss a common root");
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
        ThrowException("BoxBuilder: box body ended twice");
    _currentBox = _stack.pop();
    _currentPort = 0;
    _currentDistribution = 0;
    return *this;
}

BoxBuilder& BoxBuilder::port(QString name) {
    if (!_currentBox)
        ThrowException("BoxBuilder: port declaration outside of box context");
    _currentPort = _currentBox->port(name);
    _currentDistribution = 0;
    return *this;
}

BoxBuilder& BoxBuilder::newPort(QString name) {
    if (!_currentBox) {
        ThrowException("BoxBuilder: new port declaration out of context");
    }
    _currentPort = new Port(name, _currentBox);
    _currentPort->isBlind(true);
    _currentDistribution = 0;
    return *this;
}

BoxBuilder& BoxBuilder::rnd(QString value) {
    if (!_currentPort)
        ThrowException("BoxBuilder: rnd declaration out of context");
    value = value.simplified();
    QStringList items = value.split(" ", QString::SkipEmptyParts);
    if (items.empty())
        ThrowException("Missing distribution parameters").context(_currentPort);

    QString className = items.first();
    items.removeFirst();

    _currentDistribution = MegaFactory::create<Distribution>(className, className, _currentPort->boxParent());
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

BoxBuilder& BoxBuilder::imports(QString pathToPort) {
    if (!_currentBox)
        ThrowException("BoxBuilder: import out of context");
    _currentPort->imports(pathToPort);
    _currentDistribution = 0;
    return *this;
}

BoxBuilder& BoxBuilder::track() {
    _currentPort->track();
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
SET_ATTRIBUTE_DIRECT(label)
SET_ATTRIBUTE_DIRECT(transform)

BoxBuilder& BoxBuilder::transform(PortTransform value) {
    return attribute("transform", convert<QString>(value));
}

// Set value

BoxBuilder& BoxBuilder::equals(QString value, bool ignore) {
//    // Handle a parenthesized string as a string vector
//    if (isParenthesized(value))
//        return equals( base::split(deEmbrace(value)).toVector() );
    // Or else, handle simply as a string
    BOXBUILDER_EQUALS;
}

BoxBuilder& BoxBuilder::equals(const char *value, bool) {
    return equals(QString(value));
}

// State

const Box* BoxBuilder::currentBox() const {
    return _currentBox;
}

const Port* BoxBuilder::currentPort() const {
    return _currentPort;
}

Box* BoxBuilder::content() {
    environment().computationStep(ComputationStep::Amend);
    if (_content)
        _content->amendFamily();
    else
        ThrowException("Construction failed");
    QString info("%1 boxes created");
    dialog().information(info.arg(_content->count()));
    return _content;
}

}
