#include <boxes/mega_factory.h>
#include <boxes/port.h>
#include <savanna/lion.h>
#include "test_savanna.h"

using namespace boxes;
using savanna::Lion;

void TestSavanna::test_direct_create() {
    Box *lion = MegaFactory::create<Box>("Lion", "king", 0);
    delete lion;
//    Port *port = lion->seekPort("initialDensity");
//    QCOMPARE(port->value<double>(), 0.01);
}
