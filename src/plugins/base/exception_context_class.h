/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_EXCEPTION_CONTEXT_CLASS_H
#define BASE_EXCEPTION_CONTEXT_CLASS_H

#include <QStack>

#define ExceptionContext(X) ExceptionContextClass thisExceptionContext(X)

namespace base {
	
class ExceptionContextClass {
public:
    ExceptionContextClass(const Node *context);
    ~ExceptionContextClass();
private:
    QStack<const Node *> _contexts;
};

}
#endif
