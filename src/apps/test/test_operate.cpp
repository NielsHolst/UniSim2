#include <iostream>
#include <boost/variant/get.hpp>
#include <base/operate.h>
#include "exception_expectation.h"
#include "test_operate.h"

using std::get;
using namespace base;

void TestOperate::testAdd() {
}

void TestOperate::testAddNumbers() {
    Value a, b, c;
    a = 17;
    b = 3.12;
    c = operate::add(a,b);
    QCOMPARE(c.type(), Value::Type::Double);
    QCOMPARE(c.value<double>(), 20.12);
}

void TestOperate::testAddDate() {
    Value a, b, c;
    a = QDate(2020, 12, 24);
    b = 8;
    c = operate::add(a, b);
    QCOMPARE(c.type(), Value::Type::Date);
    QCOMPARE(c.value<QDate>(), QDate(2021, 1, 1));
}

void TestOperate::testAddTime() {
    Value a, b, c, d;
    a = QTime(14,15,16);
    b = 8;
    c = operate::add(a, b);
    QCOMPARE(c.type(), Value::Type::Time);
    QCOMPARE(c.value<QTime>(), QTime(22,15,16));

    d = 8.5;
    c = operate::add(a, d);
    QCOMPARE(c.type(), Value::Type::Time);
    QCOMPARE(c.value<QTime>(), QTime(22,45,16));
}

void TestOperate::testAddDateTime() {
    Value a, b, c, d;
    a = QDateTime(QDate(2020, 12, 24), QTime(14,15,16), Qt::UTC);
    b = 8;
    c = operate::add(a, b);
    QCOMPARE(c.type(), Value::Type::DateTime);
    QCOMPARE(c.value<QDateTime>().date(), QDate(2021, 1, 1));
    QCOMPARE(c.value<QDateTime>().time(), QTime(14,15,16));

    d = 8.5;
    c = operate::add(a, d);
    QCOMPARE(c.type(), Value::Type::DateTime);
    QCOMPARE(c.value<QDateTime>().date(), QDate(2021, 1, 2));
    QCOMPARE(c.value<QDateTime>().time(), QTime(02,15,16));
}

void TestOperate::testError() {
    bool excepted(false);
    Value a, b;
    a = QString("abc");
    b = 8;
    try {
        operate::add(a, b);
    }
    EXPECTED_EXCEPTION;
}

void TestOperate::testVector() {
    QVector<int> i;
    QVector<double> x;
    i << 4 << 2 << 3;
    x << 1.1 << 1.3 << 1.5;
    Value a, b, c;

    a = Value(i) + Value(10);
    QCOMPARE(a.type(), Value::Type::VecInt);
    QCOMPARE(a.value<vint>(), vint() << 14 << 12 << 13);

    b = Value(1.8) + Value(i);
    QCOMPARE(b.type(), Value::Type::VecDouble);
    QCOMPARE(b.value<vdouble>(), vdouble() << 5.8 << 3.8 << 4.8);

    c = Value(i) + Value(x);
    QCOMPARE(c.type(), Value::Type::VecDouble);
    QCOMPARE(c.value<vdouble>(), vdouble() << 5.1 << 3.3 << 4.5);
}
