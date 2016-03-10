#include <QTextStream>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "help.h"

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
