/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BASE_EXCEPTION_CONTEXT_CLASS_H
#define BASE_EXCEPTION_CONTEXT_CLASS_H

#include <QObject>
#include <QStack>

#define ExceptionContext(X) ExceptionContextClass thisExceptionContext(X)

namespace base {
	
class ExceptionContextClass {
public:
    ExceptionContextClass(const QObject *context);
    ~ExceptionContextClass();
private:
    QStack<const QObject *> _contexts;
};

}
#endif
