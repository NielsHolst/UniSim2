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

#define STR(X) qPrintable(convert<QString>(X))

template <class T>
auto vstr(QVector<T> v) {
    QStringList list;
    for (auto x : v)
        list << convert<QString>(x);
    return qPrintable(list.join(" "));
}

void TestReaderBoxScriptX3::testAuxAllTypes() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/aux_all_types.box"));
        root = builder.content();
    }
    UNEXPECTED_EXCEPTION;

    // Inherent ports
    try {
        QCOMPARE(root->port("iterations")->value<int>(), 1); // an input
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("iteration" )->value<int>(), 0); // an output
    }
    // Auxillary ports of all possible types of fixed values,
    // because they are fixed they should be transferred already during the construction
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("a")->value().type(), Value::Type::Bool);
        QVERIFY2(root->port("a")->value<bool>(), STR(root->port("a")->value<bool>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("b")->value().type(), Value::Type::Int);
        QVERIFY2(root->port("b")->value<int>()==117, STR(root->port("b")->value<int>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("c")->value().type(), Value::Type::Double);
        QCOMPARE(root->port("c")->value<double>(), 3.12);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("d")->value().type(), Value::Type::String);
        QCOMPARE(root->port("d")->value<QString>(), QString("abc"));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("e")->value().type(), Value::Type::Date);
        QCOMPARE(root->port("e")->value<QDate>(), QDate(2021,12,24));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("f")->value().type(), Value::Type::Time);
        QCOMPARE(root->port("f")->value<QTime>(), QTime(12,13,14));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("g")->value().type(), Value::Type::DateTime);
        QCOMPARE(root->port("g")->value<QDateTime>(), QDateTime(QDate(2021,12,24), QTime(12,13,14), Qt::UTC));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("h")->value().type(), Value::Type::BareDate);
        QCOMPARE(root->port("h")->value<BareDate>(), BareDate(12,24));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("va")->value().type(), Value::Type::VecBool);
        QVERIFY2(root->port("va")->value<vbool>()==vbool()<<true<<false<<true, vstr(root->port("va")->value<vbool>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vb")->value().type(), Value::Type::VecInt);
        QVERIFY2(root->port("vb")->value<vint>()==vint()<<133<<-28<<36, vstr(root->port("vb")->value<vint>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vc")->value().type(), Value::Type::VecDouble);
        QCOMPARE(root->port("vc")->value<vdouble>(), vdouble()<<3.12<<34.56);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vd")->value().type(), Value::Type::VecString);
        QCOMPARE(root->port("vd")->value<vQString>(), vQString()<<"abc"<<"xyz");
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("ve")->value().type(), Value::Type::VecDate);
        QCOMPARE(root->port("ve")->value<vQDate>(), vQDate()<<QDate(2021,12,24)<<QDate(2000,12,31));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vf")->value().type(), Value::Type::VecTime);
        QCOMPARE(root->port("vf")->value<vQTime>(), vQTime()<<QTime(12,13,14)<<QTime(20,21,22));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vg")->value().type(), Value::Type::VecDateTime);
        QCOMPARE(root->port("vg")->value<vQDateTime>(), vQDateTime()
                 <<QDateTime(QDate(2021,12,24), QTime(12,13,14), Qt::UTC)
                 <<QDateTime(QDate(2021,12,24), QTime(12,13,15), Qt::UTC));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vh")->value().type(), Value::Type::VecBareDate);
        QCOMPARE(root->port("vh")->value<vBareDate>(), vBareDate()<<BareDate(12,24)<<BareDate(12,31)<<BareDate(2,29));
    }
    UNEXPECTED_EXCEPTION;
}
