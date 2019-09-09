/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
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
        msg += "\nfound in your intput folder '" + env.dir(Environment::Input).absolutePath() + "'";

    dialog().information(msg);
}


}
