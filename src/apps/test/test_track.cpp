#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command.h>
#include <base/environment.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include "test_box_cases.h"
#include "test_track.h"

using namespace base;
using std::cout;

void TestTrack::testScalar() {
    Box *sim;
    try {
        sim = TestBoxCases::case3a();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }

    QSet<QString> expected;
    expected << "step" << "iteration"
             << "input2" << "input3" << "input4"
             << "output1" << "output2" << "output3";
    checkColumnNames(expected);
    delete sim;
}

void TestTrack::testVector() {
    Box *sim;
    try {
        sim = TestBoxCases::case4();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }

    QSet<QString> expected;
    expected << "step" << "iteration"
             << "numbers1"
             << "numbers2_0" << "numbers2_1" << "numbers2_2";
    checkColumnNames(expected);
    checkColumnFormat(expected.size());
    checkColumnData(expected.size());
    delete sim;
}

void TestTrack::testTrackPtr() {
    QVector<int>
        output1, output2,
        expected1, expected2;
    try {
        output1 = runJump( buildJump(17, 8) );
        output2 = runJump( buildJump(32, 8) );
    }
    catch (Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    expected1 << 17 << 52 << 26 << 13 << 40 << 20 << 10 << 5;
    expected2 << 32 << 16 <<  8 <<  4 <<  2 <<  1 <<  4 << 2;
    QCOMPARE(output1, expected1);
    QCOMPARE(output2, expected2);
//    for (int x : output1)
//        cout << x << " ";
//    cout << "\n";
//    for (int x : output2)
//        cout << x << " ";
//    cout << "\n";
}

base::Box* TestTrack::buildJump(int start, int length) {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            port("steps").equals(length-1).
            box("Jump").name("jump").
                port("initial").equals(start).
            endbox().
            box("OutputR").
                box("PageR").
                    box("PlotR").
                        port("ports").equals("(jump[value])").
                    endbox().
                endbox().
            endbox().
        endbox();
    return builder.content();
}

QVector<int> TestTrack::runJump(base::Box *sim) {
    sim->run();

    Port *value = sim->findOne<Port>("jump[value]");
    Track *track = Track::find(Track::Order{value, PortFilter::None});
    const Vector* vector = track->buffer();
    const QVector<int> *values = reinterpret_cast<const QVector<int> *>(vector->ptr());
    return *values;
}

void TestTrack::openFile() {
    QString filePath = environment().latestOutputFilePath("txt");
    file.setFileName(filePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    QVERIFY(fileOk);
}

QString toString(QSet<QString> set) {
    return QStringList(set.toList()).join(" ");
}

void TestTrack::checkColumnNames(QSet<QString> expected) {
    openFile();
    QString line = file.readLine().trimmed();
    QStringList names = line.split("\t");
    QSet<QString> columnNames;
    for (QString name : names)
        columnNames << name;
    file.close();
    if (columnNames != expected) {
        QString s{"Sets differ got(%1) != expected(%2)"};
        QFAIL(qPrintable(s.arg(toString(columnNames)).arg(toString(expected))));
    }
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
