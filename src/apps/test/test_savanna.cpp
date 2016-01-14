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

void TestSavanna::testImportOnce() {
    double killRateLion, killRateZebra;
    try {
        Box *box = TestBoxCases::case2();
        box->run();
        Box *lionAdult = Path("lion/adult").resolveOne<Box>(this),
            *zebra = Path("zebra").resolveOne<Box>(this);
        killRateLion = lionAdult->port("killRate")->value<double>();
        killRateZebra = zebra->port("killRate")->value<double>();
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    QCOMPARE(killRateLion, killRateZebra);
}
