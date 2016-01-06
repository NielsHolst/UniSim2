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
    std::cout << "test_create A\n";
    auto mother = new Box("Mother",0),
        boy = new Box("Boy", mother),
        daughter = new Box("Daughter", mother);
    std::cout << "test_create B\n";
    QCOMPARE(mother->objectName(), QString("Mother"));
    QCOMPARE(boy->objectName(), QString("Boy"));
    QCOMPARE(daughter->objectName(), QString("Daughter"));
    QCOMPARE(fullName(mother), QString("/Mother"));
    QCOMPARE(fullName(boy), QString("/Mother/Boy"));
    std::cout << "test_create Y\n";
    delete mother;
    std::cout << "test_create Z\n";
}

void TestBox::test_run() {
    std::cout << "test_run A\n";
    unique_ptr<Box> lion( MegaFactory::create<Box>("Lion", "king", 0) );
    std::cout << "test_run B\n";

    lion->run();
    std::cout << "test_run C\n";
    QCOMPARE(lion->port("iteration")->value<int>(), 1);
    std::cout << "test_run D\n";
    QCOMPARE(lion->port("step")->value<int>(), 1);
    std::cout << "test_run Z\n";
}
