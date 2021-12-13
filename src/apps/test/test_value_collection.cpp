#include <iostream>
#include <variant>
#include <base/value_collection.h>
#include "exception_expectation.h"
#include "test_value_collection.h"

using namespace base;

void TestValueCollection::testEmpty() {
    ValueCollection c;
    QVERIFY(c.isEmpty());
}

void TestValueCollection::testOne() {
    bool excepted(false);
    Value i(7);
    QVector<const Value*> ptrs;
    ptrs << &i;

    ValueCollection coll;
    try {
        coll.initialize(ptrs);
        coll.update();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(coll.type(), Value::Type::VecInt);
    vint val = coll.values().as<vint>();
    QCOMPARE(val.size(), 1);
    QCOMPARE(val.at(0), 7);
}


void TestValueCollection::testMany() {
    bool excepted(false);
    Value i(7), j(9), k(13);
    QVector<const Value*> ptrs;
    ptrs << &i << &j << &k;

    ValueCollection coll;
    try {
        coll.initialize(ptrs);
        coll.update();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(coll.type(), Value::Type::VecInt);
    vint val = coll.values().as<vint>();
    QCOMPARE(val.size(), 3);
    QCOMPARE(val, QVector<int>() << 7 << 9 << 13);
}

void TestValueCollection::testUpdate() {

}
