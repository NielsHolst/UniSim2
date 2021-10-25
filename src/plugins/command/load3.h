/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LOAD3_H
#define LOAD3_H
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace command {

class load3 : public base::Command
{
public:
    load3(QString name, QObject *parent);
private:
    // Methods
    void doExecute();
    void readFile(QString fileName);
};

}

#endif
