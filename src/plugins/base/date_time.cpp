/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "date_time.h"

namespace base {

// Use this to ensure time is in UTC
QDateTime makeDateTime(QDate date, QTime time) {
    QDateTime dt;
    dt.setDate(date);
    dt.setTime(time);
    dt.setTimeSpec(Qt::UTC);
    return dt;
}


}

