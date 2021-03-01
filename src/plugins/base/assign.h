/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_ASSIGN_H
#define BASE_ASSIGN_H
#include "port_transform.h"
#include "port_type.h"

class QObject;

namespace base {

void assign(PortType destT, void *destPtr, PortType sourceT, const void *sourcePtr, PortTransform transform, const QObject *context);

} //namespace

#endif
