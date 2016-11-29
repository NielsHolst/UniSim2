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
#include "input_file_path.h"

using namespace base;

void TestAstBoxes::init() {
    builder = new BoxBuilder ;
    reader = new BoxReaderBoxes(builder);
}

void TestAstBoxes::cleanup(){
    delete builder;
    delete reader;
}

void TestAstBoxes::testSimple() {
    try {
        reader->parse(inputFilePath("ast_simple.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    Box *root = builder->content();
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testComments() {
    try {
        reader->parse(inputFilePath("ast_comments.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    Box *root = builder->content();
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testWhitespaceInVector() {
    try {
        reader->parse(inputFilePath("ast_whitespace_in_vector.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    Box *root = builder->content();

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

void TestAstBoxes::testBadTransform() {
    bool excepted{false};
    try {
        reader->parse(inputFilePath("ast_transform.box"));
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestAstBoxes::testDistribution() {
    try {
        reader->parse(inputFilePath("ast_distribution.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
}


