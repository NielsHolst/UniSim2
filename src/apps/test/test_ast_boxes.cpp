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
    Box *root;
    try {
        reader->parse(inputFilePath("ast_simple.box"));
        root = builder->content();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testComments() {
    Box *root;
    try {
        reader->parse(inputFilePath("ast_comments.box"));
        root = builder->content();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testWhitespaceInVector() {
    Box *root;
    try {
        reader->parse(inputFilePath("ast_whitespace_in_vector.box"));
        root = builder->content();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }

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

void TestAstBoxes::testWhitespaceInPath() {
    Box *root;
    Port *x, *y, *z;
    try {
        reader->parse(inputFilePath("ast_whitespace_in_path.box"));
        root = builder->content();
        root->run();
        x = root->resolveOne<Port>("a[input2]");
        y = root->resolveOne<Port>("a[input1]");
        z = root->resolveOne<Port>("a [ input2 ]");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }

    QCOMPARE(x->value<int>(), 7);
    QCOMPARE(y->value<int>(), 3);
    QCOMPARE(z->value<int>(), 7);
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
    Box *root;
    Port *input1, *input2;
    try {
        reader->parse(inputFilePath("ast_distribution.box"));
        root = builder->content();
        root->run();
        input1 = root->resolveOne<Port>("a[input1]");
        input2 = root->resolveOne<Port>("a[input2]");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    QCOMPARE(input1->value<int>(), 20);
    QCOMPARE(input2->value<int>(), 25);
}


