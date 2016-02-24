#ifndef PORT_H
#define PORT_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include "assign.h"
#include "box_step.h"
#include "convert.h"
#include "exception.h"
#include "initialize.h"
#include "port_transform.h"
#include "port_type.h"
#include "vector.h"

namespace base {

class Box;

class Port : public QObject {
public:
    enum Access{Read=1, Write=2};
private:
    void *_valuePtr;
    PortType _valueType, _importType;
    PortTransform _transform;
    QString _importPath;
    QVector<Port *> _importPorts;
    unsigned _accessFlags;
    QString _label, _Rformat, _page, _plot;
    bool _reset;
    Vector _track;
    bool _trackOn;
    static unsigned _trackFlags;

public:
    // Configure
    Port(QString name="noname", QObject *parent=0);
    template <class T> Port& data(T *valuePtr);
    template <class T> Port& equals(T value);
    Port& equals(const char *value) { return equals(QString(value)); }
    Port& imports(QString pathToPort);
    Port& transform(PortTransform t);
    Port& access(unsigned accessFlags);
    Port& label(QString la);
    Port& Rformat(QString format);
    Port& page(QString pa);
    Port& plot(QString pl);
    Port& zeroAtReset();
    Port& zeroAtInitialize();
    Port& noReset();
    Port& trackOn();
    Port& trackOff();

    // Change
    void resolveImports();
    void allocatePortBuffer();
    void reset();
    void copyFromImport();
    void assign(const QVector<Port *> &sources);
    void track(Step step);
    void Rformat(PortType type);

    // Access
    Box *boxParent();
    bool hasValue() const;
    template <class T> T value() const;
    template <class T> const T* valuePtr() const;
    const Vector* trackPtr() const;

    // Attributes
    PortType type() const;
    PortTransform transform() const;
    unsigned accessFlags() const;
    QString label() const;
    QString Rformat() const;
    QString page() const;
    QString plot() const;
    bool hasImport() const;
    QString importPath() const;
    QVector<Port*> importPorts() const;
    static QVector<Port*> trackedPorts();
    static PortType commonType(const QVector<Port *> &ports);
};


template <class T> Port& Port::data(T *valuePtr) {
    _valuePtr = valuePtr;
    _valueType = typeOf<T>();
    Rformat(_valueType);
    base::initialize(_valueType, _valuePtr);
    return *this;
}

template <class T> Port& Port::equals(T value)
{
    if (_valuePtr == 0)
        throw Exception("Cannot set port value by 'equals' because port 'data' has not been set", "", this);
    if (!(_accessFlags & Write))
        throw Exception("Cannot set port value by 'equals' because port is not for input", "", this);
    _importPath.clear();
    base::assign(_valueType, _valuePtr, typeOf<T>(), &value, _transform);
    return *this;
}

template <class T> T Port::value() const
{
    T value;
    base::assign(typeOf<T>(), &value, _valueType, _valuePtr);
    return value;
}

template <class T> const T* Port::valuePtr() const
{
    if (_valueType != typeOf<T>()) {
        QString msg("Port of type %1 cannot be converted to type %2");
        throw Exception(msg.arg(nameOf(_valueType)).arg(nameOf(typeOf<T>())), "", this);
    }
    return reinterpret_cast<const T*>(_valuePtr);
}

template <> const void* Port::valuePtr() const;

}
#endif
