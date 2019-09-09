/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_VALUE_OP_H
#define BASE_PORT_VALUE_OP_H
#include "port_type.h"

namespace port_value_op {

// Creational
void* allocate(base::PortType type);
void deallocate(base::PortType type, void *v);

// Unary
void accumulate(base::PortType type, void *v, const void *x);
void divide(base::PortType type, void *v, double x);

// Binary
void add(base::PortType, void *v, const void *x, const void *y);
void min(base::PortType, void *v, const void *x, const void *y);
void max(base::PortType, void *v, const void *x, const void *y);
bool lt(base::PortType, const void *x, const void *y);
bool gt(base::PortType, const void *x, const void *y);
bool eq(base::PortType, const void *x, const void *y);
bool le(base::PortType, const void *x, const void *y);
bool ge(base::PortType, const void *x, const void *y);

} //namespace

#endif
