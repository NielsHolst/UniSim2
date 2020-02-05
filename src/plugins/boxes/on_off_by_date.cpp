/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/any_year.h>
#include <base/publish.h>
#include "on_off_by_date.h"

using namespace base;

namespace boxes {

PUBLISH(OnOffByDate)

OnOffByDate::OnOffByDate(QString name, QObject *parent)
    : OnOffBase<QDate>(name, parent)
{
    port("x")->unit("Date");
    port("xOn")->unit("Date");
    port("xOff")->unit("Date");
}

void OnOffByDate::setSwitch() {
    bool isWithinYear = isBefore(xOn, xOff);
    isOn = isWithinYear ?
                isAfterOrEquals(x, xOn) && isBeforeOrEquals(x, xOff) :
                isAfterOrEquals(x, xOn) || isBeforeOrEquals(x, xOff);
}

void OnOffByDate::setValue() {
    value = isOn ? valueOn : valueOff;
}


}
