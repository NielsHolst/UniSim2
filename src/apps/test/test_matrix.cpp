#include <base/data_frame.h>
#include "input_file_path.h"
#include "test_matrix.h"

using namespace base;

void TestMatrix::testInt() {
    DataFrame df;
    QVector<int> i;
    try {
        df.read( inputFilePath("matrix/matrix-int.txt"), Table::BothLabelled);
        i = df.row<int>(0);
        QCOMPARE(i, QVector<int>() << 7 << 9);
        i = df.row<int>(1);
        QCOMPARE(i, QVector<int>() << 88 << 111);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestMatrix::testString() {
    DataFrame df;
    QVector<QString> s;
    try {
        df.read( inputFilePath("matrix/matrix-string.txt"), Table::BothLabelled);
        s = df.row<QString>(0);
        QCOMPARE(s, QVector<QString>() << "7" << "9");
        s = df.row<QString>(1);
        QCOMPARE(s, QVector<QString>() << "A B"  << "111");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}
