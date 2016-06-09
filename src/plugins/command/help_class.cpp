#include <base/box.h>
#include <base/command_help.h>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/exception.h>
#include <base/factory_plug_in.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include <base/publish.h>
#include "help_class.h"

using namespace base;

namespace command {

PUBLISH(help_class)
HELP(help_help_class, "save grammar", "save grammar for Atom and Notepad++ editors")

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
        _box = MegaFactory::create<Box>(className, "helpObject", 0);
    }
    catch (Exception &) {
        _box = 0;
    }
}

void help_class::setNameLengths() {
    _maxPortNameLength = _maxPortTypeNameLength = 0;
    for (const Port *port : _box->resolveMany<Port>(".[*]")) {
        if (port->objectName().size() > _maxPortNameLength)
            _maxPortNameLength = port->objectName().size();
        if (nameOf(port->type()).size() > _maxPortTypeNameLength)
            _maxPortTypeNameLength = nameOf(port->type()).size();
    }
}

inline QString pad(QString s, int width) {
    return s + QString().fill(' ', width - s.size() + 1);
}

void help_class::writeHelp() {
    QString msg = _box->className() + " " + _box->help() +
            "\n\nInput:\n" +
            portsHelp(Port::Input).join("\n") +
            "\n\nOutput:\n" +
            portsHelp(Port::Output).join("\n");
    dialog().information(msg);
}

QStringList help_class::portsHelp(Port::Access access) {
    QStringList list;
    for (const Port *port : _box->resolveMany<Port>(".[*]"))
        if (port->access() == access)
            list << "." +
                    pad(port->objectName(), _maxPortNameLength) +
                    pad(nameOf(port->type()), _maxPortTypeNameLength) +
                    port->help();
    if (list.isEmpty())
        list << "none";
    return list;
}

}
