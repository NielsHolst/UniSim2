/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HELP_H
#define HELP_H
#include <base/command.h>

namespace command {

class help : public base::Command
{
public:
    help(QString name, QObject *parent);
private:
    void doExecute();
    void writeCommands();
};

}

#endif
