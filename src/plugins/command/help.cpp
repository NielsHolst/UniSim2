/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextStream>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
// #include <base/factory_plug_in.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include <base/publish.h>
#include "help.h"

using namespace base;

namespace command {

PUBLISH(help)
HELP(help, "help", "shows command, plugin or class documentation")

help::help(QString name, QObject *parent)
    : Command(name, parent)
{
    helpText("help");
}

void help::doExecute() {
    switch (_args.size()) {
    case 1:
        processArgument("");
        break;
    case 2:
        processArgument(QString(_args.at(1)));
        break;
    default:
        dialog().error("Write: 'help ?' to see the possible command formats");
    }
}

void help::processArgument(QString argument) {
    if (argument=="c" || argument=="C" || argument=="command")
        showCommands();
    else if (argument=="p" || argument=="P")
        showPlugins();
    else if (argument=="" || argument=="?")
        showHelp();
    else {
        if (getPlugIn(argument))
            showPlugin();
        else if (createBox(argument))
            showClass();
        else {
            QStringList pluginNames = MegaFactory::find(argument);
            int n = pluginNames.size();
            if (n==0) {
                dialog().error("Unknown name\n"
                           "Write: 'help ?' to see the possible command formats");
            }
            else if (n==1) {
                dialog().error("Unexpected error constructing " + argument + " in factory " + pluginNames.first());
            }
            else {
                for (int i=0; i<n; ++i)
                    pluginNames[i] += "::" + argument;
                dialog().error("Pick a specific class:\n" + pluginNames.join("\n"));
            }
        }

    }
}

void help::showCommands() {
    QString text;
    QTextStream sink(&text);
    for (CommandHelp help : CommandHelp::all()) {
        int width = help.command().size();
        QString filler = QString().fill(' ', CommandHelp::maxWidth() - width + 1);
        sink << help.command() << filler << "-" << help.description() << "\n";
    }
    text.remove(text.size()-1, 1);
    dialog().information(text);
}

void help::showPlugins() {
    // Collect plugin info
    auto plugins = MegaFactory::factories();
    LineInfo info;
    for (int i=0; i < plugins.size(); ++i) {
        QString name = plugins.at(i)->id(),
                desc = createBox(name + "Documentation") ?
                       _box->port("title")->valueAsString() : "undescribed";
        info.addLine(name, desc);
    }
    dialog().information(info.combined().join("\n"));
}

void help::showHelp() {
    QString s = "Use one of these formats:\n"
                "help    -- to show this help overview\n"
                "help ?  -- to show this help overview\n"
                "help c  -- to show all commands\n"
                "help p  -- to show all plugins\n"
                "help <class name>  -- to show specific class documentation\n"
                "help <plugin name> -- to show specific plugin documentation\n";
    dialog().information(s);
}

bool help::getPlugIn(QString name) {
    _plugIn = nullptr;
    for (auto factory : MegaFactory::factories()) {
        if (name == factory->id()) {
            _plugIn = factory;
        }
    }
    return _plugIn;
}

bool help::createBox(QString className) {
    try {
        Box::saveCurrentRoot();
        _box = MegaFactory::create<Box>(className, "helpObject", this);
        Box::restoreCurrentRoot();
    }
    catch (Exception &) {
        _box = nullptr;
    }
    return _box;
}

void help::showPlugin() {
    // Collect info on plugin
    QStringList headerLines;
    QString name = _plugIn->id();
    if (createBox(name+"Documentation")) {
        QString title = _box->port("title")->valueAsString();
        int length = title.length();
        QVector<QString>
                authorNames = _box->port("authorNames")->value<QVector<QString>>(),
                authorInstitutes = _box->port("authorInstitutes")->value<QVector<QString>>(),
                authorEmails = _box->port("authorEmails")->value<QVector<QString>>();
        QStringList authors;
        for (int i=0; i < authorNames.size(); ++i) {
            QString line = authorNames.at(i) + ", " +
                   authorInstitutes.at(i) + ", " +
                   authorEmails.at(i);
            if (line.size() > length)
                length = line.size();
            authors << line;
        }
        QString divider;
        divider.fill('-', length);
        headerLines
            << divider
            << title
            << divider
            << "Developed by"
            << authors
            << divider;
    }
    // Collect info on classes
    LineInfo info;
    for (QString name : _plugIn->inventory()) {
        createBox(_plugIn->id() + "::" + name);
        if (!_box)
            ThrowException("Cannot create object of class '" + name + "'");
        info.addLine(name, _box->help());
    }
    QStringList all = headerLines + info.combined();
    dialog().information(all.join("\n"));
}

void help::showClass() {
    setColWidths();
    QString msg =
        "\n" +
        _box->className() +
        " " + _box->help() +
        "\n\nInput:\n" +
       portsHelp(PortAccess::Input).join("\n") +
       "\n\nOutput:\n" +
       portsHelp(PortAccess::Output).join("\n");
    if (!_box->sideEffects().isEmpty())
        msg += "\n\nSide effects:\n" + sideEffects();
    dialog().information(msg);
}

namespace {
    bool showValue(const Port *port) {
        return port->access() == PortAccess::Input && !port->hasImport();
    }
}

void help::setColWidths() {
    _colWidthName = _colWidthValue = _colWidthUnit = 0;
    for (const Port *port : _box->findMany<Port>(".[*]")) {
        _colWidthName = std::max(_colWidthName, port->objectName().size());
        if (showValue(port))
            _colWidthValue = std::max(_colWidthValue, port->valueAsString().size());
        _colWidthUnit = std::max(_colWidthUnit, port->unit().size());
    }
}

QStringList help::portsHelp(PortAccess access) {
    QStringList list;
    for (const Port *port : _box->findMany<Port>(".[*]")) {
        if (port->access() == access) {
            QString item;
            item = "." + port->objectName().leftJustified(_colWidthName);
            QString value =  showValue(port) ? port->valueAsString() : "";
            item += "|" + value.rightJustified(_colWidthValue);
            if (_colWidthUnit > 0)
                item += " " + port->unit().leftJustified(_colWidthUnit);
            item += "|" + port->help();
            list << item;
        }
    }
    if (list.isEmpty())
        list << "none";
    return list;
}

QString help::sideEffects() {
    QStringList lines = _box->sideEffects().split("\n");
    QString s;
    for (int i = 0; i < lines.size(); ++i)
        s += QString("(%1) %2\n").arg(i+1).arg(lines.at(i));
    return s;
}

LineInfo::LineInfo()
    : maxWidth(0)
{
}

void LineInfo::addLine(QString name, QString desc) {
    items << Item{name, desc};
    if (name.size() > maxWidth)
        maxWidth = name.size();
}

QStringList LineInfo::combined() {
    QStringList msg;
    for (Item item : items) {
        QString line = item.name.leftJustified(maxWidth+1, ' ') + "-" + item.desc;
        msg << line;
    }
    return msg;
}

}
