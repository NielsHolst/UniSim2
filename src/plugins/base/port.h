#ifndef PORT_H
#define PORT_H
#include <QObject>
#include <QMap>
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
    enum Access{Input, Output};
private:
    void *_valuePtr;
    PortType _valueType, _importType;
    QString _importPath;
    QVector<Port *> _importPorts;
    Access _access;
    bool _reset;
    Vector _track;
    static unsigned _trackFlags;
    QMap<QString, QString> _attributes; //!todo _page and _plot should be string lists

public:
    // Configure
    Port(QString name="noname", QObject *parent=0, bool orphan=false);
    template <class T> Port& data(T *valuePtr);
    template <class T> Port& equals(T value);
    Port& equals(const char *value);
    Port& equals(QStringList value);
    Port& imports(QString pathToPort);
    Port& access(Access acc);
    Port& zeroAtReset();
    Port& zeroAtInitialize();
    Port& noReset();

    // Attributes
    QStringList attributes();

    Port& attribute(QString name, QString value);
    Port& format(QString fo);
    Port& page(QString pa);
    Port& plot(QString pl);
    Port& label(QString la);
    Port& transform(QString t);
    Port& transform(PortTransform t);

    QString attribute(QString name) const;
    QString format() const;
    QString label() const;
    QString page() const;
    QString plot() const;
    QString transform() const;

    // Change
    void resolveImports();
    void allocatePortBuffer();
    void reset();
    void copyFromImport();
    void assign(const QVector<Port *> &sources);
    void track(Step step);
    void format(PortType type);

    // Access
    Box *boxParent();
    bool hasValue() const;
    template <class T> T value() const;
    template <class T> const T* valuePtr() const;
    const Vector* trackPtr() const;
    PortType type() const;
    Access access() const;
    bool doTrack() const;
    bool hasImport() const;
    QString importPath() const;
    QVector<Port*> importPorts() const;
    static QVector<Port*> trackedPorts();
    static PortType commonType(const QVector<Port *> &ports);
};

template <class T> Port& Port::data(T *valuePtr) {
    _valuePtr = valuePtr;
    _valueType = typeOf<T>();
    format(_valueType);
    base::initialize(_valueType, _valuePtr);
    return *this;
}

template <class T> Port& Port::equals(T value)
{
    if (_valuePtr == 0)
        ThrowException("Cannot set value of non-existing port").context(this);
    _importPath.clear();
    base::assign(_valueType, _valuePtr, typeOf<T>(), &value, convert<PortTransform>(transform()));
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
        ThrowException(msg.arg(nameOf(_valueType)).arg(nameOf(typeOf<T>()))).context(this);
    }
    return reinterpret_cast<const T*>(_valuePtr);
}

template <> const void* Port::valuePtr() const;

}
#endif
