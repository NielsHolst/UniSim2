#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include "exception_expectation.h"
#include <base/exception.h>
#include "test_box_builder.h"
#include "test_box_cases.h"

using namespace base;

void TestBoxBuilder::testBuild1() {
    bool excepted(false);
    base::Box *tree;
    QString s;
    QTextStream str(&s);

    try {
        tree = TestBoxCases::case1();
        tree->toText(str);
    }
    UNEXPECTED_EXCEPTION;

    std::cout
            << "\n\n"
            << qPrintable(s)
            << "\n\n";
    delete tree;
}

//void TestBoxBuilder::testMissingRoot() {
//    BoxBuilder builder;
//    bool excepted{false};
//    try {
//        builder.
//            box().name("A").
//                box().name("a").
//                endbox().
//                box().name("b").
//                endbox().
//            endbox().
//            box().name("B").
//                box().name("a").
//                endbox().
//                box().name("b").
//                endbox().
//            endbox();
//    }
//    catch (Exception &) {
//        excepted = true;
//    }
//    QVERIFY(excepted);
//}
