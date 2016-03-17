#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/box_output.h>
#include <base/exception.h>
#include "test_box_builder.h"
#include "test_box_cases.h"

using namespace base;

void TestBoxBuilder::initTestCase() {
    try {
        tree = TestBoxCases::case1();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestBoxBuilder::cleanupTestCase() {
    delete tree;
}

void TestBoxBuilder::testBuild1() {
    std::cout
            << "\n\n"
            << qPrintable(BoxOutput(tree, BoxOutput::Indented).asText())
            << "\n\n";
}

void TestBoxBuilder::testMissingRoot() {
    BoxBuilder builder;
    bool excepted{false};
    try {
        builder.
            box().name("A").
                box().name("a").
                endbox().
                box().name("b").
                endbox().
            endbox().
            box().name("B").
                box().name("a").
                endbox().
                box().name("b").
                endbox().
            endbox();
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}
