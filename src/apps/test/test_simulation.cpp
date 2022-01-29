#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_simulation.h"

void TestSimulation::testGrowth() {
//    using Type = Value::Type;
//    bool excepted(false);
//    std::unique_ptr<Box> root;
//    BoxBuilder builder;
//    ReaderBoxScript reader(&builder);
//    try {
//        reader.parse(inputFilePath("box_script/simulation.box"));
//        root = std::unique_ptr<Box>( builder.content() );
//    }
//    UNEXPECTED_EXCEPTION;

//    try {
//        root->run();
//    }
//    UNEXPECTED_EXCEPTION;

//    if (root->port("hasError")->value<bool>())
//        QFAIL(qPrintable(root->port("errorMsg")->value<QString>()));
}


