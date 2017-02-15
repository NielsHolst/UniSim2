#include <stdlib.h>
#include <base/box.h>
#include <base/command_help.h>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/exception.h>
#include <base/factory_plug_in.h>
#include <base/mega_factory.h>
#include <base/publish.h>
#include "help_class.h"

using namespace base;

namespace command {

PUBLISH(help_class)
HELP(help_help_class, "help <class name>", "shows class documentation")

help_class::help_class(QString name, QObject *parent)
    : Command(name, parent), _box(0)
{
}

inline const base::FactoryPlugIn* plugIn(QString name) {
    for (auto factory : MegaFactory::factories()) {
        if (name == factory->id())
            return factory;
    }
    return 0;
}

void help_class::doExecute() {
    if (_args.size() == 2) {
        QString className = _args.at(1);
        createBox(className);
        if (_box) {
            setNameLengths();
            writeHelp();
            _box->deleteLater();
        }
        else
            dialog().error("Class name '" + className + " 'not found");
    }
    else
        dialog().error("Write: 'help <class name>'");
}

void help_class::createBox(QString className) {
    try {
        Box::saveCurrentRoot();
        _box = MegaFactory::create<Box>(className, "helpObject", 0);
        Box::restoreCurrentRoot();
    }
    catch (Exception &) {
        _box = 0;
    }
}

void help_class::setNameLengths() {
    _maxPortNameLength = _maxValueLength = 0;
    for (const Port *port : _box->findMany<Port>(".[*]")) {
        _maxPortNameLength = std::max(_maxPortNameLength, port->objectName().size());
        _maxValueLength = std::max(_maxValueLength, port->valueAsString().size());
    }
}

inline QString value(const Port *port) {
    return port->hasImport() ? port->importPath() : port->valueAsString();
}

void help_class::writeHelp() {
    QString msg = "\n" + _box->className() + " " + _box->help() +
            "\n\nInput:\n" +
            portsHelp(PortAccess::Input).join("\n") +
            "\n\nOutput:\n" +
            portsHelp(PortAccess::Output).join("\n");
    if (!_box->sideEffects().isEmpty())
        msg += "\n\nSide effects:\n" + sideEffects();
    dialog().information(msg);
}

QStringList help_class::portsHelp(PortAccess access) {
    QStringList list;
    for (const Port *port : _box->findMany<Port>(".[*]")) {
        if (port->access() == access) {
            QString item;
            item = "." + port->objectName().leftJustified(_maxPortNameLength);
            QString value =  (access == PortAccess::Input) ? port->valueAsString() : "";
            item += "|" + value.rightJustified(_maxValueLength);
            item += "|" + port->help();
            list << item;
        }
    }
    if (list.isEmpty())
        list << "none";
    return list;
}

QString help_class::sideEffects() {
    QStringList lines = _box->sideEffects().split("\n");
    QString s;
    for (int i = 0; i < lines.size(); ++i)
        s += QString("(%1) %2\n").arg(i+1).arg(lines.at(i));
    return s;
}

}
