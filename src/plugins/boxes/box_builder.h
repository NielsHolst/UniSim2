#ifndef BOX_BUILDER_H
#define BOX_BUILDER_H
#include <QStack>
#include <QString>
#include "exception.h"
#include "port.h"
#include "port_transform.h"

namespace boxes {

class Box;

class BoxBuilder
{
public:
    BoxBuilder();
    BoxBuilder& box(QString className="Box");
    BoxBuilder& name(QString boxName);
    BoxBuilder& port(QString name);
    BoxBuilder& newPort(QString name);
    BoxBuilder& import(QString pathToPort);
    BoxBuilder& endbox();
    template <class T> BoxBuilder& data(T *value);
    template <class T> BoxBuilder& equals(T value);
    BoxBuilder& transform(PortTransform pt);
    Box* content();
private:
    Box *_content, *_currentBox;
    Port *_currentPort;
    QStack<Box*> _stack;
};

template <class T> BoxBuilder& BoxBuilder::data(T *valuePtr) {
    if (!_currentPort)
        throw Exception("BoxBuilder: 'data' must follow 'port'");
    _currentPort->data(valuePtr);
    return *this;
}

template <class T> BoxBuilder& BoxBuilder::equals(T value) {
    if (!_currentPort)
        throw Exception("BoxBuilder: 'equals' must follow 'port'");
    _currentPort->equals(value);
    return *this;
}

}
#endif
