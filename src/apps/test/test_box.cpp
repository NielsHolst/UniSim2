#include <iostream>
#include <memory>
#include <base/box.h>
#include <base/general.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include "test_box.h"
#include "test_box_cases.h"

using std::unique_ptr;
using namespace base;

void TestBox::testCreate() {
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

void TestBox::testNoRun() {
    unique_ptr<Box> lion( MegaFactory::create<Box>("Lion", "king", 0) );
    bool excepted{false};
    try {
        lion->run();
    }
    catch(Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestBox::testVectorInput() {
    Box *root;
    Port *port1, *port2;
    QVector<int> expected1, expected2;
    expected1 << 7;
    expected2 << 7 << 9 << 13;
    try {
        root = TestBoxCases::case4();
        port1 = Path("V[numbers1]", root).resolveOne<Port>(root);
        port2 = Path("V[numbers2]", root).resolveOne<Port>(root);
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.what()) << "\n";
        QFAIL("Unexpected exception");
    }
    QCOMPARE(port1->value<QVector<int>>(), expected1);
    QCOMPARE(port2->value<QVector<int>>(), expected2);
    root->deleteLater();
}



