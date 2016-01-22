#include <base/port_type.h>
#include "test_port_type.h"

using namespace base;

void TestPortType::testInt() {
    QCOMPARE(typeOf<int>(), Int);
}

void TestPortType::testDate() {

}

void TestPortType::testIntVector() {

}

void TestPortType::testDateVector() {

}
