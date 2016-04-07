#include <iostream>
#include <QFile>
#include <QSet>
#include <base/box.h>
#include <base/command.h>
#include <base/environment.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include "test_box_cases.h"
#include "test_track.h"

using namespace base;


void TestTrack::testDefault() {
    bool excepted{false};
    Environment &env(environment());
    try {
        env.state.root = TestBoxCases::case3a();
        env.state.root->run();
        checkColumnNames();
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.what()) << "\n";
        excepted = true;
    }
    QVERIFY(!excepted);
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
    expected << "step" << "input2" << "input3" << "input4"
                       << "output1" << "output2" << "output3";
    QCOMPARE(result, expected);
}
