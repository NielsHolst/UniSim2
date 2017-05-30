#ifndef BASE_BOX_BUILDER_H
#define BASE_BOX_BUILDER_H
#include <QList>
#include <QObject>
#include <QStack>
#include <QString>
#include "distribution.h"
#include "exception.h"
#include "port.h"
#include "port_transform.h"

namespace base {

class Box;

class BoxBuilder
{
public:
    BoxBuilder(Box *parent=0);
    ~BoxBuilder();
    void clear();
    // Box
    BoxBuilder& box(QString className="Box");
    BoxBuilder& name(QString boxName);
    BoxBuilder& endbox();
    // Port
    BoxBuilder& port(QString name);
    BoxBuilder& newPort(QString name);
    BoxBuilder& imports(QString pathToPort);
    BoxBuilder& track(PortFilter filter=PortFilter::None);
    // Distribution
    BoxBuilder& rnd(QString name);
    BoxBuilder& mean(double value);
    BoxBuilder& sd(double value);
    BoxBuilder& min(double value);
    BoxBuilder& max(double value);
    BoxBuilder& lowerQuantile(double value);
    BoxBuilder& upperQuantile(double value);
    // Attributes by name
    BoxBuilder& attribute(QString name, QString value);
    // Attributes direct
    BoxBuilder& format(QString value);
    BoxBuilder& label(QString la);
    BoxBuilder& transform(QString tr);
    BoxBuilder& transform(PortTransform value);
    // Set value
    template <class T> BoxBuilder& data(T *value);
    template <class T> BoxBuilder& equals(T value, bool ignore=false);
    BoxBuilder& equals(QString value, bool ignore=false);
    BoxBuilder& equals(const char *value, bool ignore=false);
    // State
    const Box* currentBox() const;
    const Port* currentPort() const;
    const Distribution* currentDistribution() const;
    enum AmendOption{AmendFamily, AmendDescendants, AmendNone};
    Box* content(AmendOption amendOption=AmendFamily);
private:
    // Data
    bool _hasParent;
    Box *_content, *_currentBox;
    Port *_currentPort;
    Distribution *_currentDistribution;
    QStack<Box*> _stack;
};

template <class T> BoxBuilder& BoxBuilder::data(T *valuePtr) {
    if (!_currentPort)
        ThrowException("BoxBuilder: 'data' must follow 'port'");
    _currentPort->data(valuePtr);
    return *this;
}

#define BOXBUILDER_EQUALS \
    if (!ignore && !_currentPort) \
        ThrowException("BoxBuilder: 'equals' must follow 'port'"); \
    _currentPort->equals(value); \
    return *this

template <class T> BoxBuilder& BoxBuilder::equals(T value, bool ignore) {
    BOXBUILDER_EQUALS;
}

}
#endif
