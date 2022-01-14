/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <base/box.h>
#include <base/exception.h>
#include "output_summary.h"

using namespace base;

namespace boxes {

OutputSummary::OutputSummary() {
    _summaries << Type::Current;
}

OutputSummary::OutputSummary(QVector<QString> summaries, Box *parent)
    : _parent(parent)
{
    static QMap<QString, Type> lookup {
        {"current" , Type::Current},
        {"sum"     , Type::Sum},
        {"mean"    , Type::Mean},
        {"min"     , Type::Min},
        {"max"     , Type::Max},
        {"minat"   , Type::MinAt},
        {"maxat"   , Type::MaxAt}
    };
    for (auto s : summaries) {
        QString key = s.toLower();
        if (lookup.contains(key))
            ThrowException("Unknown summary option").value(s).context(_parent);
        _summaries << lookup.value(key);
    }
}

}
