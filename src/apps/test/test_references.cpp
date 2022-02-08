#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/exception.h>
#include <base/expression.h>
#include <base/reader_boxscript.h>
#include <base/resolved_references.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_references.h"

#define STR(X) qPrintable(convert<QString>(X))

namespace {
    auto str(QString s) {
        return qPrintable(s);
    }
}

void TestReferences::testForward() {
    using Type = Value::Type;
    bool excepted(false);
    std::unique_ptr<Box> root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/reference_forward.box"));
        root = std::unique_ptr<Box>( builder.content() );
//        std::cout << "A[a] A[b] "
//                  << str(root->findOne<Port*>("A[a]")->value().typeName()) << " "
//                  << str(root->findOne<Port*>("A[b]")->value().typeName()) << std::endl;
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("A[a]")->value().type() == Type::Date,
             str(root->findOne<Port*>("A[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[a]")->value<QDate>(), QDate(2021,12,24));

        QVERIFY2(root->findOne<Port*>("B[a]")->value().type() == Type::Date,
             str(root->findOne<Port*>("B[a]")->value().typeName()));

        QVERIFY2(root->findOne<Port*>("B[b]")->value().type() == Type::Date,
             str(root->findOne<Port*>("B[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("B[b]")->value<QDate>(), QDate(2021,12,24));

        QVERIFY2(root->findOne<Port*>("C[a]")->value().type() == Type::Null,
             str(root->findOne<Port*>("C[a]")->value().typeName()));

        QVERIFY2(root->findOne<Port*>("C[b]")->value().type() == Type::Date,
             str(root->findOne<Port*>("C[b]")->value().typeName()));

        QVERIFY2(root->findOne<Port*>("C[c]")->value().type() == Type::Date,
             str(root->findOne<Port*>("C[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("C[c]")->value<QDate>(), QDate(2021,12,24));

        QVERIFY2(root->findOne<Port*>("D[a]")->value().type() == Type::Double,
             str(root->findOne<Port*>("D[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("D[a]")->value<double>(), 13.1412);

        QVERIFY2(root->findOne<Port*>("E[a]")->value().type() == Type::Double,
             str(root->findOne<Port*>("E[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("E[a]")->value<double>(), 23.1412);

        QVERIFY2(root->findOne<Port*>("E[b]")->value().type() == Type::Double,
             str(root->findOne<Port*>("E[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("E[b]")->value<double>(), 13.1412);

        QVERIFY2(root->findOne<Port*>("F[a]")->value().type() == Type::Int,
             str(root->findOne<Port*>("F[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("F[a]")->value<int>(), 30);

        QVERIFY2(root->findOne<Port*>("F[b]")->value().type() == Type::Double,
             str(root->findOne<Port*>("F[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("F[b]")->value<double>(), 23.1412);

        QVERIFY2(root->findOne<Port*>("F[c]")->value().type() == Type::Double,
             str(root->findOne<Port*>("F[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("F[c]")->value<double>(), 13.1412);
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->resetFamily();
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->updateFamily();
        root->updateFamily();
        root->updateFamily();
    }
    UNEXPECTED_EXCEPTION;
}

void TestReferences::testUpdate() {
    using Type = Value::Type;
    bool excepted(false);
    std::unique_ptr<Box> root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/reference_update.box"));
        root = std::unique_ptr<Box>( builder.content() );
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("A[aaa]")->value().type() == Type::Uninitialized,
             str(root->findOne<Port*>("A[aaa]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[aaa]")->value<int>(), 0);
    }
    UNEXPECTED_EXCEPTION;

    root->run();
    QCOMPARE(root->findOne<Port*>("A[aaa]")->value<int>(), 140);

}
