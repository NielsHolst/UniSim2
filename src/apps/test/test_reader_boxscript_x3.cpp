#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_reader_boxscript_x3.h"


void TestReaderBoxScriptX3::testAuxAllTypes() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/aux_all_types.box"));
        root = builder.content();
    }
    UNEXPECTED_EXCEPTION;
}
