#ifndef BASE_BOX_BUILDER_H
#define BASE_BOX_BUILDER_H
#include <QList>
#include <QObject>
#include <QStack>
#include <QString>
#include "exception.h"
#include "port.h"
#include "port_transform.h"

namespace base {

class Box;

class BoxBuilder
{
public:
    BoxBuilder();
    // Box
    BoxBuilder& box(QString className="Box");
    BoxBuilder& name(QString boxName);
    BoxBuilder& endbox();
    // Port
    BoxBuilder& port(QString name);
    BoxBuilder& newPort(QString name);
    BoxBuilder& imports(QString pathToPort);
    // Attributes by name
    BoxBuilder& attribute(QString name, QString value);
    // Attributes direct
    BoxBuilder& format(QString value);
    BoxBuilder& page(QString pa);
    BoxBuilder& plot(QString pl);
    BoxBuilder& label(QString la);
    BoxBuilder& transform(QString tr);
    BoxBuilder& transform(PortTransform value);
    // Set value
    template <class T> BoxBuilder& data(T *value);
    template <class T> BoxBuilder& equals(T value);
    BoxBuilder& equals(QString value);
    BoxBuilder& equals(const char *value);
    // State
    const Box* currentBox() const;
    const Port* currentPort() const;
    Box* content();
private:
    // Data
    Box *_content, *_currentBox;
    Port *_currentPort;
    QStack<Box*> _stack;
};

template <class T> BoxBuilder& BoxBuilder::data(T *valuePtr) {
    if (!_currentPort)
        ThrowException("BoxBuilder: 'data' must follow 'port'");
    _currentPort->data(valuePtr);
    return *this;
}

#define BOXBUILDER_EQUALS \
    if (!_currentPort) \
        ThrowException("BoxBuilder: 'equals' must follow 'port'"); \
    _currentPort->equals(value); \
    return *this

template <class T> BoxBuilder& BoxBuilder::equals(T value) {
    BOXBUILDER_EQUALS;
}

}
#endif
