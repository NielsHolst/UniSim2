#include <base/box.h>
#include <base/command_help.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "debug.h"

using namespace base;

namespace command {

PUBLISH(debug)
HELP(debug, "debug", "runs simulation in debug mode")

debug::debug(QString name, QObject *parent)
    : Command(name, parent)
{
}

void debug::doExecute() {
    if (_args.size() > 2)
        ThrowException("Command 'debug' takes at most one argument");
    QStringList com;
    com << "run";
    if (_args.size() == 2)
        com << _args[1];
    Box::debug(true);
    Command::submit(com, this);
    Box::debug(false);
}

}
