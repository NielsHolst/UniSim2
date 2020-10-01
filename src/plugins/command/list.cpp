/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/box_output.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/mega_factory.h>
#include <base/factory_plug_in.h>
#include <base/publish.h>
#include "list.h"
#include "list_output.h"

using namespace base;

namespace command {

PUBLISH(list)
HELP(list, "list", "lists boxes, ports and plugins")

list::list(QString name, QObject *parent)
    : Command(name, parent)
{
}

void list::doExecute() {
    // Drop "list"
    _args.removeAt(0);

    // Extract path (or plugin name)
    // Naked 'list'
    if (_args.isEmpty())
        _path.clear();
    else if (_args.first() == "P") {
        // Special case with a 'P' path and possibly options to follow
        if(_args.size() > 1) {
            _path = "P";
            _args.removeAt(0);
        }
        // Just 'list P' with no options
        else {
            _path.clear();
        }
    }
    // Some path and possibly options to follow
    else {
        _path = _args.first();
        _args.removeAt(0);
    }

    if (_path == "?")
        showHelp();
    else {
        extractOptions();
        if (_options.contains(ListOption::Plugin))
            listPlugin();
        else
            listBoxes();
    }
}

void list::showHelp() {
    QString s = "Use one of three formats:\n"
                "list (which defaults to list . br) -or-\n"
                "list <path> (which defaults to list <path> br) -or-\n"
                "list <path> <options> -or-\n"
                "list P -or-\n"
                "list <plugin> P"
            "\n\n<options> (which can be combined) are\n"
            "p (to show Ports)\n"
            "i (to show Input ports)\n"
            "o (to show Output ports)\n"
            "m (to show iMported ports)\n"
            "x (to show eXported ports)\n"
            "b (to show Boxes)\n"
            "r (to show Recursively)\n\n"
            "P (to show Plugins or boxes inside Plugin)\n";
     dialog().information(s);
}

void list::extractOptions() {
    QString ops = _args.join("");
    _options = convert<ListOptionSet>(ops);
    if (_options.contains(ListOption::Plugin) && _options.size() > 1)
        ThrowException("Option P for plugins cannot be combined with other options").value(ops);
}

void list::listBoxes() {
    Box *current = environment().current();
    if (!current) {
        dialog().error("No script loaded");
        return;
    }
    if (_path.isEmpty())
        _path = ".";
    QVector<Box*> boxes = current->findMany<Box>(_path);
    if (boxes.isEmpty()) {
        dialog().error("Nothing matches: '" + _path +"'");
        return;
    }

    // Show output
    ListOutput output(boxes, _options);
    dialog().information(output.toString());
}

void list::listPlugin() {
    // Look up plugin names
    QStringList names, pluginNames;
    auto plugins = MegaFactory::factories();
    for (int i=0; i < plugins.size(); ++i)
        pluginNames << plugins.at(i)->id();
    if (_path.isEmpty())
        names = pluginNames;
    else {
        FactoryPlugIn *plugin = nullptr;
        for (int i=0; i < plugins.size(); ++i) {
            if (plugins.at(i)->id() == _path) {
                plugin = plugins.at(i);
                break;
            }
        }
        if (plugin)
            names = QStringList(plugin->inventory());
        else
            ThrowException("Unknown plugin name").value(_path).
                    hint("Known plugins:\n" + pluginNames.join("\n"));
    }
    dialog().information(names.join("\n"));
}

}
