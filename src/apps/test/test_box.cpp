#include <boxes/box.h>
#include <boxes/general.h>
#include "test_box.h"

using namespace boxes;

void TestBox::test_create() {
    auto mother = new Box("Mother",0),
        boy = new Box("Boy", mother),
        daughter = new Box("Daughter", mother);
    QCOMPARE(mother->objectName(), QString("Mother"));
    QCOMPARE(boy->objectName(), QString("Boy"));
    QCOMPARE(daughter->objectName(), QString("Daughter"));
    QCOMPARE(fullName(mother), QString("/Mother"));
    QCOMPARE(fullName(boy), QString("/Mother/Boy"));
    delete mother;
}
