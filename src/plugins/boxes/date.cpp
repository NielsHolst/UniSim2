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
    Output(value).help("The date constructed");
}


void Date::reset() {
    update();
}

void Date::update() {
    value = QDate(year, month, day);
    if (!value.isValid()) {
        QString s("%1/%2/%3");
        ThrowException("Invalid data (y/m/d").value(s.arg(year).arg(month).arg(year)) .context(this);
    }
}

}
