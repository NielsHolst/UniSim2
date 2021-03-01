/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "assign.h"
#include "port.h"
#include "vectorize.h"

#define CASE_VECTOR(x) case x: case x##Vector
namespace base {

const void* vectorize(PortType destType, const QVector<Port*> &ports) {
    switch (destType) {
    CASE_VECTOR(Bool):
        return vectorize<bool>(ports);
    CASE_VECTOR(Char):
        return vectorize<char>(ports);
    CASE_VECTOR(Int):
        return vectorize<int>(ports);
    CASE_VECTOR(LongInt):
        return vectorize<long int>(ports);
    CASE_VECTOR(LongLongInt):
        return vectorize<long long int>(ports);
    CASE_VECTOR(Float):
        return vectorize<float>(ports);
    CASE_VECTOR(Double):
        return vectorize<double>(ports);
    CASE_VECTOR(LongDouble):
        return vectorize<long double >(ports);
    CASE_VECTOR(String):
        return vectorize<QString>(ports);
    CASE_VECTOR(Date):
        return vectorize<QDate>(ports);
    CASE_VECTOR(Time):
        return vectorize<QTime>(ports);
    CASE_VECTOR(DateTime):
        return vectorize<QDateTime>(ports);
    case Null:
        ThrowException("Cannot convert to 'Null' type");
    }
    return 0;
}

}
