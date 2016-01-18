#include <iostream>
#include <boxes/general.h>
#include <boxes/mega_factory.h>
#include <boxes/path.h>
#include <boxes/port.h>
#include <savanna/lion.h>
#include "test_box_cases.h"
#include "test_savanna.h"

using namespace boxes;
using savanna::Lion;

void TestSavanna::testDirectCreate() {
    Box *lion = MegaFactory::create<Box>("Lion", "king", 0);
    QVERIFY(lion);
    QCOMPARE(lion->objectName(), QString("king"));
    QCOMPARE(className(lion), QString("Lion"));
    delete lion;
}

void TestSavanna::testPort() {
    Box *lion = MegaFactory::create<Box>("Lion", "king", 0);
    Port *port;
    try {
        port = lion->port("initialDensity");
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    QVERIFY(port);
    QCOMPARE(port->value<double>(), 0.01);
    delete lion;
}

void TestSavanna::testPortDefaultValue() {
    Box *lion = MegaFactory::create<Box>("Lion", "king", 0);
    Port *port;
    try {
        port = lion->port("n");
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    QVERIFY(port);
    QCOMPARE(port->value<int>(), 0);
    delete lion;
}

void TestSavanna::testNoPort() {
    bool excepted{false};
    Box *savanna;
    try {
        savanna = TestBoxCases::case2();
        Box *lion = Path("lion").resolveOne<Box>(this);
        lion->port("killRate");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete savanna;
}

void TestSavanna::testNewPort() {
    Box *savanna;
    try {
        savanna = TestBoxCases::case2();
        Box *lion = Path("lion",savanna).resolveOne<Box>(this);
        Port *port = lion->port("sum");
        QCOMPARE(port->value<int>(), 77);
        savanna->run();
        QCOMPARE(port->value<int>(), 77);
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    delete savanna;
}

void TestSavanna::testNewPortSum() {
    Box *savanna;
    try {
        savanna = TestBoxCases::case2();
        Box *lion = Path("lion",savanna).resolveOne<Box>(this);
        Port *port = lion->port("sum2");
        QCOMPARE(port->value<int>(), 0);
        savanna->run();
        QCOMPARE(port->value<int>(), 32);
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    delete savanna;
}

void TestSavanna::testImport() {
    Box *savanna;
    double killRateLion, killRateZebra;
    try {
        savanna = TestBoxCases::case2();
        savanna->run();
        Box *lionAdult = Path("lion/adult").resolveOne<Box>(this),
            *zebra = Path("zebra").resolveOne<Box>(this);
        killRateLion = lionAdult->port("killRate")->value<double>();
        killRateZebra = zebra->port("killRate")->value<double>();
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    QCOMPARE(killRateLion, killRateZebra);
    delete savanna;
}
