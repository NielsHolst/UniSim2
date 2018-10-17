/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BASE_EXCEPTION_CONTEXT_H
#define BASE_EXCEPTION_CONTEXT_H

class QObject;

namespace base {
	
class ExceptionContext {
public:
    ExceptionContext(const QObject *context);
    ~ExceptionContext();
};

}
#endif
