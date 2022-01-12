#include <QDate>
#include <QDateTime>
#include <QTime>
#include <base/bare_date.h>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_reader_boxscript_x3.h"


void TestReaderBoxScriptX3::testAuxAllTypes() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/aux_all_types.box"));
        root = builder.content();
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;
    try {
        // Inherent ports
        QCOMPARE(root->port("iterations")->value<int>(), 1);
        // Auxillary ports
        QVERIFY (root->port("a")->value<bool>());
        QCOMPARE(root->port("b")->value<int>(), 117);
        QCOMPARE(root->port("c")->value<double>(), 3.12);
        QCOMPARE(root->port("d")->value<QString>(), QString("abc"));
        QCOMPARE(root->port("e")->value<QDate>(), QDate(2021,12,24));
        QCOMPARE(root->port("f")->value<QTime>(), QTime(12,13,14));
        QCOMPARE(root->port("g")->value<QDateTime>(), QDateTime(QDate(2021,12,24), QTime(12,13,14), Qt::UTC));
        QCOMPARE(root->port("h")->value<BareDate>(), BareDate(12,24));
        QCOMPARE(root->port("va")->value<vbool>(), vbool()<<true<<false<<true);
        QCOMPARE(root->port("vb")->value<vint>(), vint()<<117<<-32<<6);
        QCOMPARE(root->port("vc")->value<vdouble>(), vdouble()<<3.12<<34.56);
        QCOMPARE(root->port("vd")->value<vQString>(), vQString()<<"abc"<<"xyz");
        QCOMPARE(root->port("ve")->value<vQDate>(), vQDate()<<QDate(2021,12,24)<<QDate(2000,12,31));
        QCOMPARE(root->port("vf")->value<vQTime>(), vQTime()<<QTime(12,13,14)<<QTime(20,21,22));
        QCOMPARE(root->port("vg")->value<vQDateTime>(), vQDateTime()
                 <<QDateTime(QDate(2021,12,24), QTime(12,13,14), Qt::UTC)
                 <<QDateTime(QDate(2021,12,24), QTime(12,13,15), Qt::UTC));
        QCOMPARE(root->port("vh")->value<vBareDate>(), vBareDate()<<BareDate(12,24)<<BareDate(12,31)<<BareDate(2,29));
    }
    UNEXPECTED_EXCEPTION;
}
