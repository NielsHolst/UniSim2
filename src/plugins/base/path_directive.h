/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PATH_DIRECTIVE_H
#define PATH_DIRECTIVEH

#include <QMap>
#include <QString>

namespace base {

namespace PathDirectives {
    enum Directive {
        Self, Children, Parent, Nearest,
        Descendants, Ascendants,
        AllSiblings, OtherSiblings, PreceedingSibling, FollowingSibling};
    Directive fromString(QString);
    QMap<QString, Directive> _pathDirectives;
}

}

#endif
