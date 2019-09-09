/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CLEAR_H
#define CLEAR_H
#include <base/command.h>

namespace command {

class clear : public base::Command
{
public:
    clear(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
