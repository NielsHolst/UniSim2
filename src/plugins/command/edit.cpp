/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDesktopServices>
#include <QUrl>
#include <base/command_help.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "edit.h"

using namespace base;

namespace command {

PUBLISH(edit)
HELP(edit, "edit", "opens box script in editor")

edit::edit(QString name, Box *parent)
    : Command(name, parent)
{
}

void edit::doExecute() {
    Environment &env(environment());
    if (_args.size() > 1)
        ThrowException("The 'edit' command takes no arguments");
    QString url = "file:///" + env.inputFileNamePath(env.latestLoadArg());
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (!ok)
        ThrowException("Could not open editor");
}

}
