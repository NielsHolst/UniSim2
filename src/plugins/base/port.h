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
#include "track.h"

#include "expression_operand.h"
#include "expression_stack.h"

namespace base {

class Box;

class Port : public QObject, public ConstructionStep {
public:
private:
    PortMode _mode;
    PortAccess _access;
    int _evaluationOrder;
    expression::Operand _value; // Never a pointer
    expression::Stack _unresolvedExpression, _expression;
    QVector<Port *> _importPorts, _exportPorts; // Register of connections
    bool _importsResolved;
    Caller _importCaller;
    bool _reset, _isBlind;
    QStringList _warnings;

public:
    // Configure
    Port(QString name="noname", QObject *parent=nullptr);
    template <class T> Port& data(T *valuePtr);
    template <class T> Port& equals(T value);
    Port& equals(const char *value);
    Port& imports(QString pathToPort, Caller caller=Caller());
    Port& access(PortAccess acc);
    Port& zeroAtReset();
    Port& noReset();

    Port& help(QString value);
    Port& unit(QString value);
    Port& isBlind(bool on);

    QString help() const;
    QString unit() const;
    bool isBlind() const;

    // Names and id
    QString name() const;
    QString fullName() const;
    int id() const;

    // Change
    void enumerate(int &number);
    void resolveImports();
    void resolveImportsAgain();
    void addExportPort(Port *port);
    void reset();
    void update();
    void copyFromImport();
    void checkSelfImport();
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
    expression::Operand value() const;
    expression::Operand::Type type() const;
    PortAccess access() const;
    bool hasImport() const;
    QVector<Port*> importPorts() const;
    QVector<Port*> exportPorts() const;
    QStringList warnings() const;
    void toText(QTextStream &text, int indentation = 0) const;
};

template <class T> Port& Port::equals(T value)
{
    _mode = PortMode::SetFixed;
    _value = value;
    return *this;
}

template <class T> T Port::value() const
{
    return _value.convert<T>();
}

template <class T> Port& Port::data(T *valuePtr) {
    _mode = PortMode::SetFixed;
    _value = valuePtr;
    return *this;
}

}
#endif
