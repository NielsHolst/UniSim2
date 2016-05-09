#include <base/box.h>
#include <base/box_builder.h>
#include "test_new_port.h"

using namespace base;

void TestNewPort::testImportNewPort() {
    BoxBuilder builder;
    Box *root;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(2).
                box("ModelA").name("A").
                    port("input1").imports("B[x]").
                    port("input2").imports("C[x]").
                    box().name("B").
                        newPort("x").equals(1234).
                    endbox().
                    box().name("C").
                        newPort("x").equals(5678).
                    endbox().
                endbox().
                box("OutputR").
                endbox().
            endbox();
        root = builder.content();

        root->run();
        QCOMPARE(root->resolveOne<Port>("A[input1]")->value<int>(), 1234);
        QCOMPARE(root->resolveOne<Port>("A[input2]")->value<int>(), 5678);
    }
    catch (Exception &ex) {
        QString msg = QString("Unexpected exception") + ex.what();
        QFAIL(qPrintable(msg));
    }
}

void TestNewPort::testImportNewPortVector() {
    BoxBuilder builder;
    Box *root;
    QVector<int> expected1, expected2;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(2).
                box("VectorInput").name("A").
                    port("numbers1").imports("B[x]").
                    port("numbers2").imports("C[x]").
                    box().name("B").
                        newPort("x").equals("(12 34)").
                    endbox().
                    box().name("C").
                        newPort("x").equals("(5 67 8 9)").
                    endbox().
                endbox().
                box("OutputR").
                endbox().
            endbox();
        root = builder.content();

        root->run();
        expected1 << 12 << 34;
        expected2 << 5 << 67 << 8 << 9;
        QCOMPARE(root->resolveOne<Port>("A[numbers1]")->value<QVector<int>>(), expected1);
        QCOMPARE(root->resolveOne<Port>("A[numbers2]")->value<QVector<int>>(), expected2);
    }
    catch (Exception &ex) {
        QString msg = QString("Unexpected exception") + ex.what();
        QFAIL(qPrintable(msg));
    }
}
