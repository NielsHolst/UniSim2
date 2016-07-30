#include <iostream>
#include <base/box.h>
#include <base/command.h>
#include <base/environment.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include "test_box_cases.h"
#include "test_track.h"

using namespace base;


void TestTrack::testScalar() {
    try {
        run(TestBoxCases::case3a());
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }

    QSet<QString> expected;
    expected << "step"
             << "input2" << "input3" << "input4"
             << "output1" << "output2" << "output3";
    checkColumnNames(expected);
    environment().deleteRoot();
}

void TestTrack::testVector() {
    try {
        run(TestBoxCases::case4());
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }

    QSet<QString> expected;
    expected << "step"
             << "numbers1"
             << "numbers2_0" << "numbers2_1" << "numbers2_2";
    checkColumnNames(expected);
    checkColumnFormat(expected.size());
    checkColumnData(expected.size());
    environment().deleteRoot();
}

void TestTrack::run(Box *simulation) {
    Environment &env(environment());
    try {
        env.root(simulation);
        env.root()->run();
    }
    catch (Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
}

void TestTrack::openFile() {
    QString filePath = environment().latestOutputFilePath("txt");
    file.setFileName(filePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    QVERIFY(fileOk);
}

void TestTrack::checkColumnNames(QSet<QString> expected) {
    openFile();
    QString line = file.readLine().trimmed();
    QStringList names = line.split("\t");
    QSet<QString> columnNames;
    for (QString name : names)
        columnNames << name;
    file.close();
    QCOMPARE(columnNames, expected);
}

void TestTrack::checkColumnFormat(int expected) {
    openFile();
    file.readLine();
    QStringList items = QString(file.readLine().trimmed()).split("\t");
    file.close();
    QCOMPARE(items.size(), expected);
}

void TestTrack::checkColumnData(int expected) {
    openFile();
    file.readLine();
    file.readLine();
    for (int i = 0; i < 3; ++i) {
        QStringList items = QString(file.readLine().trimmed()).split("\t");
        QCOMPARE(items.size(), expected);
    }
    QString empty = file.readLine().trimmed();
    file.close();
    QVERIFY(empty.isEmpty());
}
