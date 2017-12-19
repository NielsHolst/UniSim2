#include <stdio.h>
#include <QFileInfo>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "what.h"

using namespace base;

namespace command {

PUBLISH(what)
HELP(what, "what", "shows name of current script")

what::what(QString name, QObject *parent)
    : Command(name, parent)
{
}


void what::doExecute() {
    Environment &env(environment());
    if (_args.size() > 1)
        ThrowException("'what' takes no arguments");

    QString fileName =  env.latestLoadArg();
    QFileInfo fi(fileName);
    QString msg = "Current script is '" + fileName + "'";
    if (!fi.isAbsolute())
        msg += "\nfound in your work folder '" + env.dir(Environment::Work).absolutePath() + "'";

    dialog().information(msg);
}


}
