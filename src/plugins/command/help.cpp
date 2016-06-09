#include <QTextStream>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "help.h"
#include "help_class.h"

using namespace base;

namespace command {

PUBLISH(help)
HELP(help, "help", "this help")

help::help(QString name, QObject *parent)
    : Command(name, parent)
{
    helpText("help");
}

void help::doExecute() {
    Command *command(0);
    switch (_args.size()) {
    case 1:
        writeCommands();
        break;
    case 2:
        command = new help_class("help_class", this);
        break;
    default:
        dialog().error("Write: 'help' or 'help <class name>'");
    }
    if (command) {
        command->arguments(_args);
        command->execute();
    }
}

void help::writeCommands() {
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

}
