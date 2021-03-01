/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_FILTER_H
#define BASE_PORT_FILTER_H
#include <QMap>
#include <QString>
#include "convert.h"
#include "exception.h"

namespace base {

enum class PortFilter {
    None,
    Sum,
    Mean,
    Min,
    Max,
    End
};

template<class T> T convert(PortFilter)  { ThrowException("Cannot only convert port filter to QString"); }

template<> QString convert(PortFilter filter);
template<> PortFilter convert(QString s);

QStringList portFilterNames();

}

#endif
