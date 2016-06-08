#include <QDate>
#include <base/any_year.h>
#include "test_any_year.h"

using namespace base;

void TestAnyYear::testUnary() {
    QDate date(2015, 7, 4);

    QVERIFY(!hasAnyYear(date));

    setAnyYear(date);
    QVERIFY(date.isValid());
    QVERIFY(hasAnyYear(date));
}

void TestAnyYear::testBinaryOneHasAnyYear() {
    QDate dateAnyYear(2015, 7, 4),
          dateEqual(2016, 7, 4),
          dateBefore(2016, 7, 1),
          dateAfter(2014, 7, 10);
    setAnyYear(dateAnyYear);

    QVERIFY(equals(dateAnyYear, dateEqual));
    QVERIFY(isBefore(dateBefore, dateAnyYear));
    QVERIFY(isAfter(dateAfter, dateAnyYear));
}
