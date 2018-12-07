/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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

