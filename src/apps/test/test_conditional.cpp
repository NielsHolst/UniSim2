#include <QSet>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "exception_expectation.h"
#include "test_conditional.h"

using namespace base;

void TestConditional::testSimple() {
    int errors = dialog().errorCount();
    Box *sim;
    try {
        Command::submit(QStringList() << "load" << "conditional/simple.box", nullptr);
        sim = environment().root();
        sim->initializeFamily();
        sim->resetFamily();
    }
    UNEXPECTED
    QCOMPARE(errors, dialog().errorCount());

    double a = sim->findOne<Box>("A")->port("duration")->value<double>();
    double b = sim->findOne<Box>("B")->port("duration")->value<double>();

    QCOMPARE(a, 30.);
    QCOMPARE(b, 17.);

    delete sim;
}
