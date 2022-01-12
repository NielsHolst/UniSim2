#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include "exception_expectation.h"
#include "test_box_builder.h"
#include "test_box_cases.h"

using namespace base;

void TestBoxBuilder::testBuild1() {
    bool excepted(false);
    base::Box *tree, *root;
    QString s;
    QTextStream str(&s);

    try {
        tree = TestBoxCases::case1();
        root = Box::root();
        root->initializeFamily();
        root->toText(str);
    }
    catch(const base::Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    catch (const std::exception& ex) {
        QString s = "Unexpected exception: " + QString(ex.what());
        QFAIL(qPrintable(s));
    }
    catch (...) {
        QFAIL("Unexpected exception");
    }
    QVERIFY(!excepted);
    excepted = false;




    std::cout
            << "\n\n"
            << qPrintable(s)
            << "\n\n";
    delete tree;
}
