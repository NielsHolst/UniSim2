/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VECTOR_H
#define BASE_VECTOR_H
#include "port_type.h"

namespace base {

class Port;

class Vector {
public:
    Vector(Port *parent);
    ~Vector();
    void reserve(int size);
    void erase();
    void append(const void *valuePtr);
    QString toString(int row, QString separator = "\t") const;
    int size() const;
    PortType type() const;
    const void *ptr() const;
private:
    PortType _vectorType;
    void *_vectorPtr;
    Port *_parent;
};

}
#endif
