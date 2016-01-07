#include <iostream>
#include <memory>
#include <boxes/box.h>
#include <boxes/general.h>
#include <boxes/mega_factory.h>
#include <boxes/port.h>
#include "test_box.h"

using std::unique_ptr;
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

void TestBox::test_run() {
    unique_ptr<Box> lion( MegaFactory::create<Box>("Lion", "king", 0) );
    lion->run();
    QCOMPARE(lion->port("iteration")->value<int>(), 1);
    QCOMPARE(lion->port("step")->value<int>(), 1);
}
