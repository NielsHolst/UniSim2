#include <QSet>
#include <QVector>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include "output_file.h"
#include "test_maker.h"

using namespace base;

void TestMaker::testScript() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "maker/maker.box", nullptr);
    QCOMPARE(errors, dialog().errorCount());

    Box *sim = environment().root();
    QVector<Box*> islands = sim->findMany<Box*>("islands/*");
    QCOMPARE(islands.size(), 3);

    const Box *island0 = islands.at(0);
    const Box *island1 = islands.at(1);
    const Box *island2 = islands.at(2);
    QCOMPARE(island0->objectName(), QString("island0"));
    QCOMPARE(island1->objectName(), QString("island1"));
    QCOMPARE(island2->objectName(), QString("island2"));

    for (Box *island : islands) {
        QVector<Box*> populations = island->findMany<Box*>("./*<Population>");
        QCOMPARE(populations.size(), 2);
        QCOMPARE(populations.at(0)->objectName(), QString("fox"));
        QCOMPARE(populations.at(1)->objectName(), QString("rabbit"));
    }

    // Must run OK
    try {
        sim->run();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QCOMPARE(errors, dialog().errorCount());
    QWARN(qPrintable(OutputFile().filePath()));
}
