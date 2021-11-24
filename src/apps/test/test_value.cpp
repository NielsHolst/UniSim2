#include <iostream>
#include <QString>
#include <base/value.h>
#include "exception_expectation.h"
#include "test_value.h"

using namespace base;

void TestValue::testBoolTyped() {
    ValueTyped<bool> val;

    val.initialize(8);
    QCOMPARE(val.value<bool>(), true);
    QCOMPARE(val.value<int>(), 1);
    QCOMPARE(val.value<QString>(), QString("1"));

    val.changeValue(QString());
    QCOMPARE(val.value<bool>(), false);

    val.changeValue(QString("abc"));
    QCOMPARE(val.value<bool>(), true);

    const bool *x = val.valuePtr();
    QCOMPARE(*x, true);
}


void TestValue::testInitializeByValue() {
    Value a, b;

    a.initialize(17);
    b.initialize(3.12);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.value<int>(), 17);
    QCOMPARE(b.value<double>(), 3.12);

    const int    *x = a.valuePtr<int>();
    const double *y = b.valuePtr<double>();

    a.changeValue(3.12);
    b.changeValue(17);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.value<int>(), 3);
    QCOMPARE(b.value<double>(), 17.0);

    QCOMPARE(*x, 3);
    QCOMPARE(*y, 17.0);
}

void TestValue::testInitializeByPointer() {
    int i = 17;
    double k = 3.12;
    Value a, b;

    a.initialize(&i);
    b.initialize(&k);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.value<int>(), 17);
    QCOMPARE(b.value<double>(), 3.12);

    const int    *x = a.valuePtr<int>();
    const double *y = b.valuePtr<double>();

    a.changeValue(3.12);
    b.changeValue(17);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(i, 3);
    QCOMPARE(k, 17.0);
    QCOMPARE(a.value<int>(), 3);
    QCOMPARE(b.value<double>(), 17.0);

    QCOMPARE(*x, 3);
    QCOMPARE(*y, 17.0);
}

void TestValue::testAssignmentByValue() {
    Value a, b;
    a = 17;
    b = 3.12;
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.value<int>(), 17);
    QCOMPARE(b.value<double>(), 3.12);
}

void TestValue::testAssignmentByPointer() {
    int i = 17;
    double k = 3.12;
    Value a, b;
    a = &i;
    b = &k;
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.value<int>(), 17);
    QCOMPARE(b.value<double>(), 3.12);
}

void TestValue::testAssignmentTwice() {
    Value a;
    a = 117;
    QCOMPARE(a.value<int>(), 117);
    a.changeValue(118);
    QCOMPARE(a.value<int>(), 118);
    a = 119;
    QCOMPARE(a.value<int>(), 119);

    // Changing a Value by assignment does not thange the type!
    a = 3.12;
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(a.value<int>(), 3);
}

void TestValue::testAssignmentToUninitialized() {
    Value a, b;
    a = "abc";
    b = a;
    QCOMPARE(a.type(), Value::Type::String);
    QCOMPARE(b.type(), Value::Type::String);
    QCOMPARE(a.value<QString>(), QString("abc"));
    QCOMPARE(b.value<QString>(), QString("abc"));
}

void TestValue::testBool() {
    bool excepted(false);
    Value val;
    bool x = false;
    const bool *y;
    try {
        val.initialize(&x);

        val.changeValue(8);
        QCOMPARE(val.value<bool>(), true);
        QCOMPARE(x, true);
        QCOMPARE(val.value<int>(), 1);
        QCOMPARE(val.value<QString>(), QString("1"));

        val.changeValue(QString());
        QCOMPARE(val.value<bool>(), false);
        QCOMPARE(x, false);

        val.changeValue(QString("abc"));
        QCOMPARE(val.value<bool>(), true);
        QCOMPARE(x, true);

        y = val.valuePtr<bool>();
        QCOMPARE(*y, true);
    }
    UNEXPECTED_EXCEPTION;
}

void TestValue::testInt() {
    bool excepted(false);
    Value val;

    int x = 117;
    val.initialize(&x);
    QCOMPARE(val.value<int>(), 117);

    const int *y = val.valuePtr<int>();
    QCOMPARE(*y, 117);

    try {
        val.valuePtr<double>();
    }
    EXPECTED_EXCEPTION;

    Value val2;
    val.initialize(42);
    QCOMPARE(val.type(), Value::Type::Int);
    QCOMPARE(val.value<int>(), 42);

    Value a, b;

    int u = 17;
    double v = 3.12;
    a.initialize(&u);
    b.initialize(&v);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.value<int>(), 17);
    QCOMPARE(b.value<double>(), 3.12);
}
