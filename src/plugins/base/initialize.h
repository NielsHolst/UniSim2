/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_INITIALIZE_H
#define BASE_INITIALIZE_H
#include "port_transform.h"
#include "port_type.h"

class QObject;

namespace base {

template <class T> void initialize(T *destPtr) { *destPtr = T(); }

void initialize(PortType destT, void *destPtr, QObject *context);

} //namespace

#endif
