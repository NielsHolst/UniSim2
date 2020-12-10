/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "date_split.h"

using namespace base;

namespace boxes {

PUBLISH(DateSplit)

DateSplit::DateSplit(QString name, QObject *parent)
    : Box(name, parent)
{
    help("splits a date into day, month and year");
    Input(date).help("The date to split");
    Output(day).equals(1).help("Day of month").unit("[1;31]");
    Output(month).equals(1).help("Month").unit("[1;12]");
    Output(year).equals(1).help("Year").unit("integer");
}


void DateSplit::reset() {
    update();
}

void DateSplit::update() {
    if (!date.isValid())
        ThrowException("Invalid date").context(this);
    day = date.day();
    month = date.month();
    year = date.year();
}

}
