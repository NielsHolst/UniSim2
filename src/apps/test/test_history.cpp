#include <iostream>
#include <console/history.h>
#include "test_history.h"

using namespace console;

void TestHistory::testAddOne() {
    History history(5);
    QCOMPARE(history.entry(0), QStringList() );
    history.add("moon stone");
    QCOMPARE(history.entry(0), QStringList() << "moon" << "stone");
}

void TestHistory::testAddMany() {
    History history(5);
    history.add("moon stone A");
    history.add("moon stone B");
    history.add("moon stone C");
    history.add("moon stone D");
    history.add("moon stone E");
    history.add("\"moon stone\" F");
    QCOMPARE(history.entry(0), QStringList() << "moon stone" << "F");
    QCOMPARE(history.entry(1), QStringList() << "moon" << "stone" << "E");
    QCOMPARE(history.entry(2), QStringList() << "moon" << "stone" << "D");
    QCOMPARE(history.entry(3), QStringList() << "moon" << "stone" << "C");
    QCOMPARE(history.entry(4), QStringList() << "moon" << "stone" << "B");
}
