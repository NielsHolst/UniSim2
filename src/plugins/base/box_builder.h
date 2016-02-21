#ifndef BOX_BUILDER_H
#define BOX_BUILDER_H
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
    BoxBuilder& label(QString la);
    BoxBuilder& axis(QString ax);
    BoxBuilder& page(QString pa);
    BoxBuilder& plot(QString pl);
    BoxBuilder& track(QString tr);
    BoxBuilder& trackOn();
    BoxBuilder& trackOff();
    BoxBuilder& Rformat(QString format);
    BoxBuilder& transform(PortTransform pt);
    // Data templates
    template <class T> BoxBuilder& data(T *value);
    template <class T> BoxBuilder& equals(T value);
    // State
    const Box* currentBox() const;
    const Port* currentPort() const;
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
