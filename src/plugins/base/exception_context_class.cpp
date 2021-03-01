/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
#include "exception_context_class.h"

namespace base {

ExceptionContextClass::ExceptionContextClass(const QObject *context) {
    Exception::setContext(context);
    _contexts.push(context);
}

ExceptionContextClass::~ExceptionContextClass() {
    const QObject *prevContext = _contexts.isEmpty() ? nullptr : _contexts.pop();
    Exception::setContext(prevContext);
}

}

