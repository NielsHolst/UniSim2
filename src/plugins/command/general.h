/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COMMAND_GENERAL
#define COMMAND_GENERAL

#include <QObject>
#include <QVector>

namespace command {

QString info(QVector<QObject*> objects);
QString info(QObject *object);

}

#endif
