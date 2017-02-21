#ifndef BASE_PORT_H
#define BASE_PORT_H
#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>
#include "assign.h"
#include "box_step.h"
#include "construction_step.h"
#include "convert.h"
#include "enum_functions.h"
#include "environment.h"
#include "exception.h"
#include "initialize.h"
#include "port_access.h"
#include "port_buffer.h"
#include "port_mode.h"
#include "port_transform.h"
#include "port_type.h"
#include "vector.h"

namespace base {

class Box;

class Port : public QObject, public ConstructionStep {
public:
    enum class ToTextOptions{None=0};
    struct Attributes {
        QString format, label, transform, help;
        PortTransform portTransform;
        Attributes() {
            transform = "Identity";
            portTransform = Identity;
        }
    };
private:
    void *_valuePtr;
    PortType _valueType, _importType;
    PortMode _mode;
    ComputationStep _portValueStep;
    QString _importPath;
    QVector<Port *> _importPorts;
    bool _importPortMustExist;
    PortAccess _access;
    bool _reset, _hasTrack, _valueOverridden;
    Vector _trackBuffer;
    static unsigned _trackFlags;
    Attributes _attributes;
    bool _isBlind;
    QStringList _warnings;

    void checkValueOverridden();

public:
    // Configure
    Port(QString name="noname", QObject *parent=0);
    template <class T> Port& data(T *valuePtr);
    template <class T> Port& equals(T value);
    Port& equals(const char *value);
    Port& equals(QStringList value);
    Port& imports(QString pathToPort);
    Port& importsMaybe(QString pathToPort);
    Port& access(PortAccess acc);
    Port& zeroAtReset();
    Port& zeroAtInitialize();
    Port& noReset();
    Port& isBlind(bool on);

    // Attributes
    static QStringList attributes();

    Port& attribute(QString name, QString value);
    Port& format(QString s);
    Port& label(QString s);
    Port& transform(QString s);
    Port& help(QString s);
    Port& transform(PortTransform t);

    QString attribute(QString name) const;
    QString format() const;
    QString label() const;
    QStringList labelList() const;
    QString help() const;
    PortTransform transform() const;
    bool isBlind() const;

    // Change
    void resolveImports();
    void allocatePortBuffer();
    void reset();
    void copyFromImport();
    void assign(const QVector<Port *> &sources);
    void track(Step step);
    void format(PortType type);
    void track();

    // Access
    Box *boxParent();
    bool hasValue() const;
    int valueSize() const;
    QString valueAsString() const;
    template <class T> T value() const;
    template <class T> const T* valuePtr() const;
    const Vector* trackPtr() const;
    PortType type() const;
    PortAccess access() const;
    bool hasTrack() const;
    bool hasImport() const;
    bool isValueOverridden() const;
    QString importPath() const;
    QVector<Port*> importPorts() const;
    QStringList warnings() const;
    void toText(QTextStream &text, ToTextOptions options, int indentation = 0) const;
    template <class T> void deducePortType(T value);
    static QVector<Port*> trackedPorts();
    static PortType commonType(const QVector<Port *> &ports);
};

DEFINE_ENUM_FUNCTIONS(Port::ToTextOptions)

template <class T> Port& Port::data(T *valuePtr) {
    _valuePtr = valuePtr;
    _valueType = typeOf<T>();
    format(_valueType);
    base::initialize(_valueType, _valuePtr);
    return *this;
}

// It the value type is unknown then it must be deduced from the value,
// either from its value as such (if it is a string), or else from its type
template <class T> void Port::deducePortType(T value) {
    if (_valueType == Null) {
        _valueType = (typeOf<T>() == String) ?
                     base::deducePortType(convert<QString>(value)) : // Superflous conversion to QString to avoid compilation type conflict
                     typeOf<T>();
    }
}

template <class T> Port& Port::equals(T value)
{
    _mode = PortMode::Fixed;
    _portValueStep = environment().computationStep();
    // Deduce the value type as necessary
    deducePortType<T>(value);
    // Create a buffer for the value if it does not exist
    if (_valuePtr == 0)
        _valuePtr = portBuffer(this).createBuffer(_valueType);
    // Copy value to the buffer that _valuePtr points to
    base::assign(_valueType, _valuePtr, typeOf<T>(), &value, transform(), this);
    checkValueOverridden();
    return *this;
}

template <class T> T Port::value() const
{
    T value;
    base::assign(typeOf<T>(), &value, _valueType, _valuePtr, PortTransform::Identity, this);
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
