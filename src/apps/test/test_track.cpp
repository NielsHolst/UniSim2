#include <iostream>
#include <QFile>
#include <QSet>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog_stub.h>
#include <base/environment.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include "test_box_cases.h"
#include "test_track.h"

using namespace base;


void TestTrack::testDefault() {
    bool excepted{false};
    Environment &env(environment());
    DialogStub *dialog = new DialogStub(0);
    try {
        env.state.root = TestBoxCases::case3a();
        env.state.root->run();
        save(&env);
        checkColumnNames();
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.what()) << "\n";
        excepted = true;
    }
    delete dialog;
    QVERIFY(!excepted);
}

void TestTrack::save(QObject *parent) {
    Command *save = MegaFactory::create<Command>("save", "save", parent);
    save->arguments(QStringList() << "save" << "output");
    save->execute();
}

void TestTrack::checkColumnNames() {
    QString filePath = environment().state.latestOutputFilePath;
    QFile file (filePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    QVERIFY(fileOk);

    QString line = file.readLine().trimmed();
    QStringList names = line.split("\t");

    QSet<QString> result, expected;
    for (QString name : names)
        result << name;
    expected << "iteration" << "step" << "input2" << "input3" << "input4" << "output1";
    QCOMPARE(result, expected);
}
