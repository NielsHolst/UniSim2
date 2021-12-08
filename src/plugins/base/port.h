/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_H
#define BASE_PORT_H
#include <boost/variant/get.hpp>
#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>
#include "caller.h"
#include "construction_step.h"
#include "convert.h"
//#include "enum_functions.h"
//#include "environment.h"
#include "exception.h"
#include "port_access.h"

#include "value.h"
#include "expression.h"

namespace base {

class Box;

class Port : public QObject, public ConstructionStep {
public:
private:
    bool
        _isValueOverridden, // Has the default value been overridden in the BoxScript?
        _doReset,           // Should the value be re-initialised (according to type) at reset?
        _isExtraneous;      // Is this a port declared in external BoxScript?
    PortAccess _access;     // Either input or output
    QString
        _unit,              // Unit text
        _help;              // Help text
    int _evaluationOrder;   // Order of evaluation among all ports
    Value
        _value;             // Holds the current value
    Expression
        _expression;        // Any path operands have been replaced with port pointers, when the expression was closed
    QVector<Port *>
        _importPorts,       // Ports imported by this port
        _exportPorts;       // Ports which imports this port
    Caller _importCaller;   // Latest object calling for an import of this port (for error message)

public:
    // Configure
    Port(QString name, QObject *parent);
    Port(const Port &x) : QObject(x.parent()) { assign(x); }
    Port& operator=(const Port &x)            { assign(x);  return *this; }
    Port& doReset();
    Port& noReset();
    Port& access(PortAccess acc);
    Port& unit(QString value);
    Port& help(QString value);
    Port& isExtraneous();

    // Set value
    template <class T> Port& initialize(T *variable);
    template <class T> Port& equals(T fixedValue);
//    template <>        Port& equals(const char *fixedValue);
//    template <>        Port& equals(Value value);
    Port& imports(QString pathToPort, Caller caller=Caller());

    // Query
    Box *boxParent();
    QString name() const;
    QString fullName() const;
    PortAccess access() const;
    QString unit() const;
    QString help() const;
    bool isValueOverridden() const;
    int evaluationOrder() const;
    QVector<Port*> importPorts() const;
    QVector<Port*> exportPorts() const;

    // Get value
    template <class T> T value() const;
    const Value& value() const;
    void verifyValue() const;
    const Expression& expression() const;

    // Housekeeping
    void addExportPort(Port *port);
    void enumerate(int &number);
    void closeExpression();
    void reset();
    void update();

    // Output
    void toText(QTextStream &text, int indentation = 0);
private:
    void assign(const Port &x);
    void checkIfValueOverridden();
};

template <class T> Port& Port::initialize(T *variable) {
    _value.initialize(variable);
    return *this;
}

template <class T> Port& Port::equals(T fixedValue)
{
    checkIfValueOverridden();
    _expression.push(Value(fixedValue));
    return *this;
}

template <class T> T Port::value() const
{
    return _value.as<T>();
}

}
#endif
