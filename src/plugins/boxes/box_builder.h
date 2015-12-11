#ifndef BOX_BUILDER_H
#define BOX_BUILDER_H
#include <QStack>
#include <QString>
#include "exception.h"
#include "port.h"

namespace boxes {

class Box;

class BoxBuilder
{
public:
    BoxBuilder();
    BoxBuilder& box(QString name);
    BoxBuilder& port(QString name);
    template <class T> BoxBuilder& equals(T value);
    BoxBuilder& endbox();
    Box* content();
private:
    Box *_content, *_currentBox;
    Port *_currentPort;
    QStack<Box*> _stack;
};

template <class T> BoxBuilder& BoxBuilder::equals(T value) {
    if (!_currentPort)
        throw Exception("BoxBuilder:'equals' must follow 'port'");
    _currentPort->equals(value);
    return *this;
}

}
#endif
