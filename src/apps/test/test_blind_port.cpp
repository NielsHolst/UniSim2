#include <iostream>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "exception_expectation.h"
#include "output_file.h"
#include "test_blind_port.h"

using namespace base;

void TestBlindPort::testFormat() {
    Box *sim;
    try {
        Command::submit(QStringList() << "run" << "blind-port-format.box", nullptr);
        sim = environment().root();
    }
    UNEXPECTED
    QCOMPARE(dialog().errorCount(), 0);

    OutputFile file;
    std::cout << "*** " << qPrintable(file.filePath()) << "\n"
              << qPrintable(file.columnLabels().join(" - ")) << "\n"
              << qPrintable(file.columnFormats().join(" - ")) << "\n";

//    QStringList values = file.column("f");
//    QCOMPARE(values.size(), 20);
//    QSet<QString> set;
//    for (QString value : values.toVector())
//        set << value;
//    QCOMPARE(set.size(), 5);
}
