#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "output_file.h"
#include "test_simulation.h"

void TestSimulation::testGrowth() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/simulation.box"));
        root = Box::root(builder.content());
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

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    if (root->port("hasError")->value<bool>())
        QFAIL(qPrintable(root->port("errorMsg")->value<QString>()));

    OutputFile f;
    QCOMPARE(f.columnLabels(), QStringList()<<"iteration"<<"sim/step"<<"comp/step"<<"r"<<"density");
    QCOMPARE(f.cell<int>(3,"sim/step"), 3);
    QCOMPARE(f.cell<double>(3,"density"), 115.763);
}


