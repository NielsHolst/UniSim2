/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_CLASS_NAME_H
#define BASE_CLASS_NAME_H
#include <QObject>
#include <QString>

namespace base {

void addClassName(QObject *object, QString className);
bool hasClassName(QObject *object, QString className);


} //namespace

#endif
