/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QObject>
#include "exception.h"
#include "exception_context.h"

namespace base {

ExceptionContext::ExceptionContext(const QObject *context) {
    Exception::setContext(context);
}

ExceptionContext::~ExceptionContext() {
    Exception::setContext(nullptr);
}

}

