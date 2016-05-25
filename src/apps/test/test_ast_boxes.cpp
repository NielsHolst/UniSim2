#include <QFile>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/box_output.h>
#include <base/box_reader_boxes.h>
#include <base/dialog.h>
#include <base/exception.h>
#include <base/port.h>
#include "test_ast_boxes.h"

using namespace base;

void TestAstBoxes::testSimple() {
    BoxBuilder builder;
    BoxReaderBase *reader = new BoxReaderBoxes(&builder);
    try {
        reader->parse(filePath("ast_simple.box"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Unexpected: " + ex.what()));
    }
    Box *root = builder.content();
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());}

QString TestAstBoxes::filePath(QString fileName) {
    return "../src/apps/test/input/" + fileName;
}


