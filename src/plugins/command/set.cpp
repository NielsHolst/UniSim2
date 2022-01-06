/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/command_help.h>
#include <base/publish.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "set.h"
#include "set_font.h"
#include "set_folder.h"

using namespace base;

namespace command {

PUBLISH(set)
HELP(set_any, "set", "shows what can be set")

set::set(QString name, QObject *parent)
    : Command(name, parent)
{
    setClassName("set");
}

void set::doExecute() {
    Command *command(0);
    int n = _args.size();
    if (n == 1) {
        dialog().error("Write: 'set folder' or 'set font'");
        return;
    }
    else {
        Q_ASSERT(n > 1);
        QString a1 = _args.at(1);
        if (a1 == "font")
            command = new set_font("set_font", this);
        else if (a1 == "folder")
            command = new set_folder("set_folder", this);
        else
            ThrowException("Unknown command").value(_args.join(" "));
    }
    if (command) {
        command->arguments(_args);
        command->execute();
    }
}


}
