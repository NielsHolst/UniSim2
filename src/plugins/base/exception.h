/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BOXES_EXCEPTION_H
#define BOXES_EXCEPTION_H

#include <exception>
#include <QString>

class QObject;

namespace base {
	
class Exception : public std::exception {
public:
    Exception(QString message, QString value = "", const QObject *context = 0, QString hint = "");
    QString message();
    QString value();
    const QObject* context();
    const char* what() const _GLIBCXX_USE_NOEXCEPT;

private:
    QString _message, _value, _hint;
    const QObject *_context;
};

}
#endif
