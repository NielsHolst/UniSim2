#include <iostream>
#include <boost/variant/get.hpp>
#include <base/expression_operation.h>
#include "exception_expectation.h"
#include "test_expression_operation.h"

using boost::get;
using namespace base;
using namespace expression;

void TestExpressionOperation::testAdd() {
    Operand a = 17,
            b = 3.12,
            c = add(a,b);
    QCOMPARE(c.type(), expression::Operand::Type::Double);
    QCOMPARE(get<double>(c), 20.12);

    a = QDate(2020, 12, 24);
    b = 8;
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::Date);
    QCOMPARE(get<QDate>(c), QDate(2021, 1, 1));


    a = QTime(14,15,16);
    b = 8;
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::Time);
    QCOMPARE(get<QTime>(c), QTime(22,15,16));

    a = QTime(14,15,16);
    b = 8.5;
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::Time);
    QCOMPARE(get<QTime>(c), QTime(22,45,16));

    a = QDateTime(QDate(2020, 12, 24), QTime(14,15,16), Qt::UTC);
    b = 8;
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::DateTime);
    QCOMPARE(get<QDateTime>(c).date(), QDate(2021, 1, 1));
    QCOMPARE(get<QDateTime>(c).time(), QTime(14,15,16));

    a = QString("abc");
    b = 8;
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::String);
    QCOMPARE(get<QString>(c), QString("abc8"));

    bool excepted(false);
    a = 'x';
    b = 8;
    try {
        c = add(a, b);
    }
    EXPECTED_EXCEPTION;
}

void TestExpressionOperation::testAddPtr() {
    Operand a, b, c;
    int x = 7, y = 8;
    double z = 8.5;

    a = &x;
    b = &y;
    c = add(a,b);
    QCOMPARE(a.type(), expression::Operand::Type::IntPtr);
    QCOMPARE(b.type(), expression::Operand::Type::IntPtr);
    QCOMPARE(c.type(), expression::Operand::Type::Int);
    QCOMPARE(*get<int*>(a), 7);
    QCOMPARE(*get<int*>(b), 8);
    QCOMPARE(get<int>(c), 15);

    a = QDate(2020, 12, 24);
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::Date);
    QCOMPARE(get<QDate>(a), QDate(2020, 12, 24));
    QCOMPARE(get<QDate>(c), QDate(2021, 1, 1));


    a = QTime(14,15,16);
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::Time);
    QCOMPARE(get<QTime>(a), QTime(14,15,16));
    QCOMPARE(get<QTime>(c), QTime(22,15,16));

    a = QTime(14,15,16);
    b = &z;
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::Time);
    QCOMPARE(get<QTime>(a), QTime(14,15,16));
    QCOMPARE(get<QTime>(c), QTime(22,45,16));

    a = QDateTime(QDate(2020, 12, 24), QTime(14,15,16), Qt::UTC);
    b = 8;
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::DateTime);
    QCOMPARE(get<QDateTime>(a), QDateTime(QDate(2020, 12, 24), QTime(14,15,16), Qt::UTC));
    QCOMPARE(get<QDateTime>(c).date(), QDate(2021, 1, 1));
    QCOMPARE(get<QDateTime>(c).time(), QTime(14,15,16));

    a = QString("abc");
    c = add(a, b);
    QCOMPARE(c.type(), expression::Operand::Type::String);
    QCOMPARE(get<QString>(c), QString("abc8"));

    bool excepted(false);
    a = 'x';
    try {
        c = add(a, b);
    }
    EXPECTED_EXCEPTION;
}
