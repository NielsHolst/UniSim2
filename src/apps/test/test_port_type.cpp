#include <boxes/port_type.h>
#include "test_port_type.h"

using namespace boxes;

void TestPortType::testInt() {
    QCOMPARE(typeOf<int>(), Int);
}

void TestPortType::testDate() {

}

void TestPortType::testIntVector() {

}

void TestPortType::testDateVector() {

}
