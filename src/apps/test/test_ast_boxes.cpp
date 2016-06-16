#include <QFile>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/box_output.h>
#include <base/box_reader_boxes.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include "test_ast_boxes.h"

using namespace base;

void TestAstBoxes::testSimple() {
    BoxBuilder builder;
    BoxReaderBase *reader = new BoxReaderBoxes(&builder);
    try {
        reader->parse(filePath("ast_simple.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    Box *root = builder.content();
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testComments() {
    BoxBuilder builder;
    BoxReaderBase *reader = new BoxReaderBoxes(&builder);
    try {
        reader->parse(filePath("ast_comments.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    Box *root = builder.content();
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testWhitespaceInVector() {
    BoxBuilder builder;
    BoxReaderBase *reader = new BoxReaderBoxes(&builder);
    try {
        reader->parse(filePath("ast_whitespace_in_vector.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    Box *root = builder.content();

    typedef QVector<int> VecInt;
    QCOMPARE(root->resolveOne<Port>("a[numbers1]")->value<VecInt>(),
             VecInt() << 7 << 9 << 13);
    QCOMPARE(root->resolveOne<Port>("a[numbers1]")->value<VecInt>(),
             VecInt() << 7 << 9 << 13);

    typedef QVector<QString> VecString;
    QCOMPARE(root->resolveOne<Port>("b[s1]")->value<VecString>(),
             VecString() << "abc" << "def" << "ghi");
    QCOMPARE(root->resolveOne<Port>("b[s2]")->value<VecString>(),
             VecString() << "abc" << "def" << "ghi");
}

QString TestAstBoxes::filePath(QString fileName) {
    return environment().filePath(Environment::Input, fileName);
}


