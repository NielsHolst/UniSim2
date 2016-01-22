#include <iostream>
#include <QVector>
#include <base/assign.h>
#include "test_assign.h"

using namespace base;

void TestAssign::testToCharFromBool() {
    char dest(0);
    bool source(true);
    assign(Char, &dest, Bool, &source);
    QCOMPARE(dest, char(1));
}

void TestAssign::testToCharFromChar() {
    char dest(0), source('K');
    assign(Char, &dest, Char, &source);
    QCOMPARE(dest, source);
}

void TestAssign::testToCharFromBoolVector() {
    char dest(0);
    QVector<bool> source{true, false, true};
    assign(Char, &dest, BoolVector, &source, Any);
    QCOMPARE(dest, char(1));
    assign(Char, &dest, BoolVector, &source, All);
    QCOMPARE(dest, char(0));
}

void TestAssign::testToCharVectorFromBoolVector() {
    QVector<char> dest{0,0,0}, expected{1,0,1};
    QVector<bool> source{true, false, true};
    assign(CharVector, &dest, BoolVector, &source);
    QCOMPARE(dest, expected);
}

void TestAssign::testToCharVectorFromCharVector() {
    QVector<char> dest{0,0,0}, source{'y','o','h'};
    assign(CharVector, &dest, CharVector, &source);
    QCOMPARE(dest, source);
}

void TestAssign::testToCharVectorFromBool() {
    QVector<char> dest{0}, expected{1};
    bool source{true};
    assign(CharVector, &dest, Bool, &source);
    QCOMPARE(dest, expected);
}

void TestAssign::testToStringFromInt() {
    QString dest;
    int source{7};
    assign(String, &dest, Int, &source);
    QCOMPARE(dest, QString("7"));
}

void TestAssign::testToStringFromIntVector() {
    QString dest;
    QVector<int> source{7, 14, 15};
    assign(String, &dest, IntVector, &source, Sum);
    QCOMPARE(dest, QString("36"));
}

void TestAssign::testToStringFromDate() {
    QString dest;
    QDate source(2015, 12, 24);
    assign(String, &dest, Date, &source);
    QCOMPARE(dest, QString("24/12/2015"));
}

void TestAssign::testToIntFromDate() {
    int dest;
    QDate source(2015, 2, 10);
    assign(Int, &dest, Date, &source);
    QCOMPARE(dest, 41);
}

void TestAssign::testToIntFromTime() {
    int dest;
    QTime source(10, 15, 0);
    assign(Int, &dest, Time, &source);
    QCOMPARE(dest, 10);
}

void TestAssign::testToDoubleFromTime() {
    double dest;
    QTime source(10, 15, 0);
    assign(Double, &dest, Time, &source); //?
    QCOMPARE(dest, 10. + 15./60.);
}

void TestAssign::testToDoubleFromDateTime() {
    double dest;
    QDateTime source = QDateTime(QDate(2015, 2, 10), QTime(6,0,0));
    assign(Double, &dest, DateTime, &source); //?
    QCOMPARE(dest, 41. + 6./24.);
}
