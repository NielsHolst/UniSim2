/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_TRANSFORM_H
#define BASE_PORT_TRANSFORM_H
#include <QMap>
#include <QString>
#include "convert.h"
#include "exception.h"

namespace base {

enum PortTransform {
    Identity,
    Sum,
    Prod,
    Mean,
    Min,
    Max,
    Copy,
    Split,
    All,
    Any
};

template<class T> T convert(PortTransform )  { ThrowException("Cannot only convert port transform to QString"); }

template<> QString convert(PortTransform transform);
template<> PortTransform convert(QString s);

QStringList portTransformNames();

}

#endif
