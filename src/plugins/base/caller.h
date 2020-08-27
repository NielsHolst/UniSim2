/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CALLER_H
#define CALLER_H

#include <QString>

#define CA Caller(this, QString(__FILE__), __LINE__)

class QObject;

namespace base {
	
class Caller {
public:
    Caller();
    Caller(const QObject *caller, QString file, int line);
    const QObject* caller() const {
        return _caller;
    }
    QString file() const {
        return _file;
    }
    int line() const {
        return _line;
    }
private:
    const QObject *_caller;
    QString _file;
    int _line;
};

}
#endif