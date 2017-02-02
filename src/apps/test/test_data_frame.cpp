#include <base/data_frame.h>
#include "input_file_path.h"
#include "test_data_frame.h"

using namespace base;

void TestDataFrame::testDimensions() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df1.txt") );
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QCOMPARE(df.ncol(), 3);
    QCOMPARE(df.nrow(), 2);
}

void TestDataFrame::testContent() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt") );
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QCOMPARE(df.ncol(), 3);
    QCOMPARE(df.nrow(), 2);

    QCOMPARE(df.colnames(), QStringList() << "ColA" << "ColB" << "ColC");
    QCOMPARE(df.row(0), QStringList() << "Row1" << "7" << "9");
    QCOMPARE(df.row(1), QStringList() << "Row2" << "A B" << "111");

    QCOMPARE(df.col(0), QStringList() << "Row1" << "Row2");
    QCOMPARE(df.col(1), QStringList() << "7" << "A B");
    QCOMPARE(df.col(2), QStringList() << "9" << "111");

    QCOMPARE(df.ixcol("ColA"), 0);
    QCOMPARE(df.ixcol("ColB"), 1);
    QCOMPARE(df.ixcol("ColC"), 2);
}
