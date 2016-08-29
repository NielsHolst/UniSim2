#ifndef BASE_ASSIGN_H
#define BASE_ASSIGN_H
#include "port_transform.h"
#include "port_type.h"

class QObject;

namespace base {

void assign(PortType destT, void *destPtr, PortType sourceT, const void *sourcePtr, PortTransform transform, const QObject *context);

} //namespace

#endif
