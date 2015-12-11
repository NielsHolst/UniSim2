#include <iostream>
#include <boxes/box.h>
#include <boxes/box_builder.h>
#include <boxes/box_output.h>
#include <boxes/exception.h>
#include "test_box_builder.h"
#include "test_box_cases.h"

using namespace boxes;

void TestBoxBuilder::initTestCase() {
    try {
        tree = TestBoxCases::case1();
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
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
