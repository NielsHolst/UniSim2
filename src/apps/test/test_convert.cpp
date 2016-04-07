#include <base/convert.h>
#include "test_convert.h"

using namespace base;

void TestConvert::testStringListFromString() {
    QString s("(A 7 X)");
    QStringList result, expected;
    expected << "A" << "7" << "X";
    result = convert<QStringList>(s);
    QCOMPARE(result, expected);
}

void TestConvert::testIntFromStringList() {
    QStringList list;
    QVector<int> expected, result;
    list << "7" << "9" << "13";
    expected << 7 << 9 << 13;
    result = convert<int, QVector>(list);
    QCOMPARE(result, expected);
}




