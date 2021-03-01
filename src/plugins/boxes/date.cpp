/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "date.h"

using namespace base;

namespace boxes {

PUBLISH(Date)

Date::Date(QString name, QObject *parent)
    : Box(name, parent)
{
    help("constructs a date from day, month and year");
    Input(day).equals(1).help("Day of month").unit("[1;31]");
    Input(month).equals(1).help("Month").unit("[1;12]");
    Input(year).equals(1).help("Year").unit("Integer");
    Output(date).help("The date constructed");
}


void Date::reset() {
    update();
}

void Date::update() {
    date = QDate(year, month, day);
    if (!date.isValid()) {
        QString s("%1/%2/%3");
        ThrowException("Invalid date (y/m/d").value(s.arg(year).arg(month).arg(year)) .context(this);
    }
}

}
