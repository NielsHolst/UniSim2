#ifndef BASE_ANY_YEAR_H
#define BASE_ANY_YEAR_H

#include <QDate>

namespace base {

int anyYear();
bool hasAnyYear(QDate date);
void setAnyYear(QDate &date);
bool equals(QDate date1, QDate date2);
bool isBefore(QDate date1, QDate date2);
bool isAfter(QDate date1, QDate date2);
bool isBeforeOrEquals(QDate date1, QDate date2);
bool isAfterOrEquals(QDate date1, QDate date2);

}

#endif

