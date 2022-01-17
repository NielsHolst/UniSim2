/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_H
#define BASE_PORT_H
#include <boost/variant/get.hpp>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>
#include "caller.h"
#include "convert.h"
#include "exception.h"
#include "expression.h"
#include "node.h"
#include "value.h"
#include "success.h"

namespace base {

class Box;

class Port : public Node {
public:
    enum class Type{Input, Output, Auxiliary};
private:
    Type _type;             // Purpose of the port
    bool
        _hasBeenRedefined, // Has the default value been overridden in the BoxScript?
        _clearAtReset,                // Is value set to T() at reset()? Defaults to true for output and aux ports
        _hasBeenFixed;           // An input port can be fixed in initialize() or later, which means the expression does
                                 // not be to evaluated again. Fixing happens if the expression is a constant value or its
                                 // evaluation involves only constants and other fixed ports
    QString
        _unit,              // Unit text
        _help;              // Help text
    Value
        _value;             // Holds the current value
    Expression
        _expression;        // Any path operands have been replaced with port pointers, when the expression was closed
    QStringList
        _outputNames;       // Unique name labelling of the columns for this port in the output text file;
                            // empty if not included in output;
                            // output summaries may result in more than one output column
    QVector<Port *>
        _importPorts,       // Ports imported by this port
        _exportPorts;       // Ports which imports this port
    Caller _importCaller;   // Latest object calling for an import of this port (for error message)

public:
    // Configure
    Port(QString name, Type type, Node *parent);
//    Port(const Port &x) : Node(x.name(), x.parent()) { assign(x); }
//    Port& operator=(const Port &x)                   { assign(x);  return *this; }
    Port& doClear();
    Port& noClear();
    Port& unit(QString value);
    Port& help(QString value);
    void outputNames(QStringList columnNames);

    // Define port
    template <class T> Port& initialize(T *variable);
    template <class T> Port& initialize(T  variable);
    Port& initialize(Value value);
    template <class T> Port& equals(T fixedValue);
    Port& equals(const Value &value);
    Port& equals(const Expression &expression);
    Port& imports(QString pathToPort, Caller caller=Caller());
    Port& computes(QString expression);

    // Change value
    void clear();
    void evaluate(Success rule);

    // Query
    Box *boxParent();
    Port::Type type() const;
    QString unit() const;
    QString help() const;
    QString importPath() const;
    int size() const;
    bool isValueOverridden() const;
    QStringList outputNames() const;
    QVector<Port*> importPorts() const;
    QVector<Port*> exportPorts() const;

    // Get value
    template <class T> T value() const;
    template <class T> const T* valuePtr() const;
    const Value& value() const;
    void verifyValue() const;
    const Expression& expression() const;
    QString format() const;

    // Housekeeping
    void addExportPort(Port *port);

    // Output
    void toText(QTextStream &text, int indentation = 0);
private:
    void define();
//    void assign(const Port &x);
};

template <class T> Port& Port::initialize(T *variable) {
    _value.initialize(variable);
    return *this;
}

template <class T> Port& Port::initialize(T fixedValue) {
    _value.initialize(fixedValue);
    equals(fixedValue);
    return *this;
}

template <class T> Port& Port::equals(T fixedValue)
{
    return equals(Value(fixedValue));
}

template <class T> T Port::value() const
{
    return _value.as<T>();
}

template <class T> const T* Port::valuePtr() const
{
    return _value.constPtr<T>();
}

template<class T> T convert(Port::Type) {
    ThrowException("Cannot convert Port::Type");
}

template<> QString convert(Port::Type type);

}
#endif
