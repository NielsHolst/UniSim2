#ifndef ASSIGN_H
#define ASSIGN_H
#include "port_transform.h"
#include "port_type.h"

namespace boxes {

void assign(PortType destT, void *destPtr, PortType sourceT, const void *sourcePtr, PortTransform transform = Identity);

} //namespace

#endif
