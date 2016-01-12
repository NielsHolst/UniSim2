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

    Port *port = lion->port("initialDensity");
    QVERIFY(port);
    QCOMPARE(port->value<double>(), 0.01);

    delete lion;
}

void TestSavanna::testImport() {
    try {
        Box *box = TestBoxCases::case2();
        box->run();
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }

    Box *lionAdult = Path("lion/adult").resolveOne<Box>(this),
        *zebra = Path("zebra").resolveOne<Box>(this);
    double killRateLion = lionAdult->port("killRate")->value<double>(),
           killRateZebra = zebra->port("killRate")->value<double>();
    QCOMPARE(killRateLion, killRateZebra);
}
