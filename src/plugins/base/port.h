/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_H
#define BASE_PORT_H
#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>
#include "assign.h"
#include "caller.h"
#include "construction_step.h"
#include "convert.h"
#include "enum_functions.h"
#include "environment.h"
#include "exception.h"
#include "initialize.h"
#include "port_access.h"
#include "port_buffer.h"
#include "port_filter.h"
#include "port_mode.h"
#include "port_transform.h"
#include "port_type.h"
#include "track.h"

namespace base {

class Box;

class Port : public QObject, public ConstructionStep {
public:
private:
    int _id;
    void *_valuePtr;
    PortType _valueType, _importType;
    PortMode _mode;
    QMap<QString, QString> _attributes;
    ComputationStep _portValueStep;
    QString _importPath, _fallBackValue;
    QVector<Port *> _importPorts, _exportPorts;
    bool _importPortMustExist, _importsResolved;
    Caller _importCaller;
    PortAccess _access;
    bool _isReference, _reset, _valueOverridden;
    bool _isBlind;
    QStringList _warnings;

    void checkValueOverridden();

public:
    // Configure
    Port(QString name="noname", QObject *parent=nullptr);
    template <class T> Port& data(T *valuePtr);
    template <class T> Port& equals(T value);
    Port& equals(const char *value);
    Port& equals(QStringList value);
    Port& imports(QString pathToPort, Caller caller=Caller());
    Port& importsMaybe(QString pathToPort, QString fallBackValue=QString(), Caller caller=Caller());
    Port& access(PortAccess acc);
    Port& reference();
    Port& zeroAtReset();
    Port& zeroAtInitialize();
    Port& noReset();

    // Attributes
    QStringList attributes();

    Port& attribute(QString name, QString value);
    Port& help(QString value);
    Port& unit(QString value);
    Port& format(QString value);
    Port& label(QString value);
    Port& transform(QString value);
    Port& transform(PortTransform t);
    Port& isBlind(bool on);

    QString attribute(QString name) const;
    QString help() const;
    QString unit() const;
    QString format() const;
    QString label() const;
    PortTransform transform() const;
    bool isBlind() const;
    bool isReference() const;

    // Names and id
    QString name() const;
    QString fullName() const;
    int id() const;

    // Change
    void resolveImports();
    void resolveImportsAgain();
    void addExportPort(Port *port);
    void reset();
    void update();
    void copyFromImport();
    void copyFromSelfImport(const Box *self);
    void assign(const QVector<Port *> &sources);
    void format(PortType type);
    Track::Order track(PortFilter filter=PortFilter::None);

    // Access
    Box *boxParent();
    bool hasValue() const;
    int valueSize() const;
    QString valueAsString() const;
    void verifyValue() const;
    template <class T> T value() const;
    template <class T> const T* valuePtr() const;
    PortType type() const;
    PortAccess access() const;
    bool hasImport() const;
    bool hasDistribution() const;
    bool isValueOverridden() const;
    QString importPath() const;
    QVector<Port*> importPorts() const;
    QVector<Port*> exportPorts() const;
    QStringList warnings() const;
    void toText(QTextStream &text, int indentation = 0) const;
    template <class T> void deducePortType(T value);
    static PortType commonType(const QVector<Port *> &ports);
    static void clearIndex();
    static Port* find(int id);
    static QVector<Port*> _index;
    static QString dump();
};

template <class T> Port& Port::data(T *valuePtr) {
    _valuePtr = valuePtr;
    _valueType = typeOf<T>();
    if (_valueType == Null)
        ThrowException("Ports not supported for this type").context(this);
    format(_valueType);
    base::initialize(_valueType, _valuePtr, this);
    return *this;
}

// It the value type is unknown then it must be deduced from its value or C++ type
template <class T> void Port::deducePortType(T value) {
    if (_valueType == Null) {
        if (hasDistribution())
            // If port has a distribution attached, it must be a double
            _valueType = Double;
        else if (typeOf<T>() == String)
            // Superflous conversion to QString to avoid compilation type conflict
            _valueType = base::deducePortType(convert<QString>(value));
        else
            // Just use its C++ type
            _valueType = typeOf<T>();
    }
}

template <class T> Port& Port::equals(T value)
{
    ExceptionContext(this);
    _isReference = false;
    _mode = PortMode::Fixed;
    _portValueStep = environment().computationStep();
    // Deduce the value type as necessary
    deducePortType<T>(value);
    // Create a buffer for the value if it does not exist
    if (_valuePtr == nullptr)
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
