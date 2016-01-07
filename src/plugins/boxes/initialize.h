#ifndef INITIALIZE_H
#define INITIALIZE_H
#include "port_transform.h"
#include "port_type.h"


namespace boxes {


template <class T> void initialize(T *destPtr) { *destPtr = T(); }

void initialize(PortType destT, void *destPtr);



} //namespace

#endif
