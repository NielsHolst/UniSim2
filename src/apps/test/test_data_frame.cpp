#include <base/data_frame.h>
#include "input_file_path.h"
#include "test_data_frame.h"

using namespace base;

void TestDataFrame::testDimensions() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df1.txt"), DataFrame::BothLabelled);
        QCOMPARE(df.numCol(), 2);
        QCOMPARE(df.numRow(), 2);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentBothLabelled() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), DataFrame::BothLabelled);
        QCOMPARE(df.numCol(), 2);
        QCOMPARE(df.numRow(), 2);

        QCOMPARE(df.colNames(), QStringList() << "ColB" << "ColC");
        QCOMPARE(df.ixCol("ColB"), 0);
        QCOMPARE(df.ixCol("ColC"), 1);

        QCOMPARE(df.rowNames(), QStringList() << "Row1" << "Row2");
        QCOMPARE(df.ixRow("Row1"), 0);
        QCOMPARE(df.ixRow("Row2"), 1);

        QCOMPARE(df.row(0), QStringList() << "7" << "9");
        QCOMPARE(df.row(1), QStringList() << "A B" << "111");

        QCOMPARE(df.col(0), QStringList() << "7" << "A B");
        QCOMPARE(df.col(1), QStringList() << "9" << "111");

        QCOMPARE(df(0,0), QString("7"));
        QCOMPARE(df(1,1), QString("111"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentColumnLabelled() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), DataFrame::ColumnLabelled);
        QCOMPARE(df.numCol(), 3);
        QCOMPARE(df.numRow(), 2);

        QCOMPARE(df.colNames(), QStringList() << "ColA" << "ColB" << "ColC");
        QCOMPARE(df.ixCol("ColA"), 0);
        QCOMPARE(df.ixCol("ColB"), 1);
        QCOMPARE(df.ixCol("ColC"), 2);

        QVERIFY(df.rowNames().isEmpty());

        QCOMPARE(df.row(0), QStringList() << "Row1" <<"7" << "9");
        QCOMPARE(df.row(1), QStringList() << "Row2" << "A B" << "111");

        QCOMPARE(df.col(0), QStringList() << "Row1" << "Row2");
        QCOMPARE(df.col(1), QStringList() << "7" << "A B");
        QCOMPARE(df.col(2), QStringList() << "9" << "111");

        QCOMPARE(df(0,0), QString("Row1"));
        QCOMPARE(df(1,1), QString("A B"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentRowLabelled() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), DataFrame::RowLabelled);
        QCOMPARE(df.numCol(), 2);
        QCOMPARE(df.numRow(), 3);

        QVERIFY(df.colNames().isEmpty());

        QCOMPARE(df.rowNames(), QStringList() << "ColA" << "Row1" << "Row2");
        QCOMPARE(df.ixRow("ColA"), 0);
        QCOMPARE(df.ixRow("Row1"), 1);
        QCOMPARE(df.ixRow("Row2"), 2);

        QCOMPARE(df.row(0), QStringList() << "ColB" << "ColC");
        QCOMPARE(df.row(1), QStringList() << "7" << "9");
        QCOMPARE(df.row(2), QStringList() << "A B" << "111");

        QCOMPARE(df.col(0), QStringList() << "ColB" << "7" << "A B");
        QCOMPARE(df.col(1), QStringList() << "ColC" << "9" << "111");

        QCOMPARE(df(0,0), QString("ColB"));
        QCOMPARE(df(1,1), QString("9"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentNoLabelling() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), DataFrame::NoLabelling);
        QCOMPARE(df.numCol(), 3);
        QCOMPARE(df.numRow(), 3);

        QVERIFY(df.colNames().isEmpty());
        QVERIFY(df.rowNames().isEmpty());
        QCOMPARE(df.row(0), QStringList() << "ColA" << "ColB" << "ColC");
        QCOMPARE(df.row(1), QStringList() << "Row1" << "7" << "9");
        QCOMPARE(df.row(2), QStringList() << "Row2" << "A B" << "111");

        QCOMPARE(df.col(0), QStringList() << "ColA" << "Row1" << "Row2");
        QCOMPARE(df.col(1), QStringList() << "ColB" << "7" << "A B");
        QCOMPARE(df.col(2), QStringList() << "ColC" << "9" << "111");

        QCOMPARE(df(0,0), QString("ColA"));
        QCOMPARE(df(1,1), QString("7"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}
