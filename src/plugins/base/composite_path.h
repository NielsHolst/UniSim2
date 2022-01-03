/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_COMPOSITE_PATH_H
#define BASE_COMPOSITE_PATH_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include "convert.h"
#include "exception.h"
#include "path.h"

namespace base {

class CompositePath {
public:
    CompositePath(const QObject *parent = nullptr);
    void addPath(const Path &path);
    void resolve();
private:
    const QObject *_parent;
    QVector<Path> _paths;
};

}

#endif
