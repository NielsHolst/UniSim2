/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QFile>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "libr.h"

using namespace base;

namespace command {

PUBLISH(libr)
HELP(libr, "libr", "shows the needed R libraries")

libr::libr(QString name, QObject *parent)
    : Command(name, parent)
{
}

void libr::doExecute() {
    QString fileNamePath = environment().filePath(Environment::Input, "scripts/install_packages.R");
    QFile file(fileNamePath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        ThrowException("Could not open file").value(fileNamePath);
    QString s = file.readAll();
    dialog().information(s);
}


}
