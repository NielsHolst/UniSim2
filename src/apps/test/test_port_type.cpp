#include <limits>
#include <base/convert.h>
#include <base/port_type.h>
#include "test_port_type.h"

using std::numeric_limits;
using namespace base;

void TestPortType::testInt() {
    QCOMPARE(typeOf<int>(), Int);
}

void TestPortType::testDate() {
    QCOMPARE(typeOf<QDate>(), Date);
}

void TestPortType::testIntVector() {

}

void TestPortType::testDateVector() {
    QCOMPARE(deducePortType("(31/12/2001)"), DateVector);
    QCOMPARE(deducePortType("(31/12/2001 2001/12/31 /12/31/2001)"), DateVector);
    QCOMPARE(deducePortType("(31/12/2001 2001/12/31 /12/31/2001 x)"), StringVector);
}

void TestPortType::testDeduceBool() {
    PortType type = deducePortType("TRUE");
    QCOMPARE(type, Bool);
//    QCOMPARE(deducePortType("(TRUE FALSE)"), BoolVector);
}


void TestPortType::testDeduceChar() {
    QCOMPARE(deducePortType("X"), Char);
    QCOMPARE(deducePortType(" "), Char);
}

void TestPortType::testDeduceInt(){
    PortType type = deducePortType("123");
    QCOMPARE(type, Int);
}

void TestPortType::testDeduceDouble() {
    PortType type = deducePortType("3.14");
    QCOMPARE(type, Double);
}

void TestPortType::testDeduceDate() {
    QCOMPARE(deducePortType("31/12/2001"), Date);
    QCOMPARE(deducePortType("2001/12/31"), Date);
    QCOMPARE(deducePortType("/12/31/2001"), Date);
}

void TestPortType::testDeduceDateTime() {
    QCOMPARE(deducePortType("31/12/2001 9:35"), DateTime);
}

void TestPortType::testDeduceTime() {
    QCOMPARE(deducePortType("9:35"), Time);
}

void TestPortType::testDeduceQString() {
    QCOMPARE(deducePortType(""), String);
    QCOMPARE(deducePortType("True"), String);
    QCOMPARE(deducePortType("(a b "), String);
    QCOMPARE(deducePortType("a38"), String);
}


