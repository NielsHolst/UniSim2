#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/dialog.h>
#include <base/port_buffer.h>
#include "test_port_buffer.h"

using namespace std;
using namespace base;

void TestPortBuffer::initTestCase() {
}

void TestPortBuffer::cleanupTestCase() {
}

void TestPortBuffer::testCreateBool() {
    bool *p = reinterpret_cast<bool*>( portBuffer().createBuffer(Bool) ),
         *q = reinterpret_cast<bool*>( portBuffer().createBuffer(Bool) );
    *p = true;
    *q = false;
    QVector<bool> *u = reinterpret_cast<QVector<bool>*>( portBuffer().createBuffer(BoolVector) ),
                  *v = reinterpret_cast<QVector<bool>*>( portBuffer().createBuffer(BoolVector) );
    *u << true << false;
    *v << false << true;
}

//
// Scalar
//

void TestPortBuffer::testImportNoBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("ModelA").name("A").
                port("input1").imports(".[input2]").
                port("input2").equals(777).
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->resolveOne<Port>("A[input1]")->value<int>(), 777);
    delete sim;
}

void TestPortBuffer::testImportFromBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("ModelA").name("A").
                port("input1").imports(".[input10]").
                newPort("input10").equals(777).
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->resolveOne<Port>("A[input1]")->value<int>(), 777);
    delete sim;
}

void TestPortBuffer::testImportToBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("ModelA").name("A").
                newPort("input10").imports(".[input2]").
                port("input2").equals(777).
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->resolveOne<Port>("A[input10]")->value<int>(), 777);
    delete sim;
}

void TestPortBuffer::testImportBufferToBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("ModelA").name("A").
                newPort("input10").imports(".[input20]").
                newPort("input20").equals(777).
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->resolveOne<Port>("A[input10]")->value<int>(), 777);
    delete sim;
}

//
// Vector
//

void TestPortBuffer::testImportVectorNoBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("VectorInput").name("A").
                port("numbers1").imports(".[numbers2]").
                port("numbers2").equals("(7 13 9)").
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QVector<int> expected;
    expected << 7 << 13 << 9;
    QCOMPARE(sim->resolveOne<Port>("A[numbers1]")->value<QVector<int>>(), expected);
    delete sim;
}

void TestPortBuffer::testImportVectorFromBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("VectorInput").name("A").
                port("numbers1").imports(".[numbers20]").
                newPort("numbers20").equals("(7 13 9)").
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QVector<int> expected;
    expected << 7 << 13 << 9;
    QCOMPARE(sim->resolveOne<Port>("A[numbers1]")->value<QVector<int>>(), expected);
    delete sim;
}

void TestPortBuffer::testImportVectorToBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("VectorInput").name("A").
                newPort("numbers10").imports(".[numbers2]").
                port("numbers2").equals("(7 13 9)").
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QVector<int> expected;
    expected << 7 << 13 << 9;
    QCOMPARE(sim->resolveOne<Port>("A[numbers10]")->value<QVector<int>>(), expected);
    delete sim;
}

void TestPortBuffer::testImportVectorBufferToBuffer() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("VectorInput").name("A").
                newPort("numbers10").imports(".[numbers20]").
                newPort("numbers20").equals("(7 13 9)").
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QVector<int> expected;
    expected << 7 << 13 << 9;
    QCOMPARE(sim->resolveOne<Port>("A[numbers10]")->value<QVector<int>>(), expected);
    delete sim;
}
