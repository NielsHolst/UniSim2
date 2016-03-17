#include <iostream>
#include <base/box.h>
#include <base/dialog_stub.h>
#include <base/general.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include "test_box_cases.h"
#include "test_savanna.h"

using namespace base;

void TestSavanna::initTestCase() {
    dialogStub = new DialogStub(0);
}

void TestSavanna::cleanupTestCase() {
    dialogStub->deleteLater();
}

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
        QFAIL(qPrintable(ex.what()));
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
        QFAIL(qPrintable(ex.what()));
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
        delete savanna;
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
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
        delete savanna;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
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
        delete savanna;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
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
        delete savanna;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QCOMPARE(killRateLion, killRateZebra);
}
