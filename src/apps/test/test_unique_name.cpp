#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/path.h>
#include <base/port.h>
#include <base/unique_name.h>
#include "test_unique_name.h"

using namespace base;
using namespace std;

void TestUniqueName::testMany() {
    BoxBuilder builder;
    builder.
    box().name("root").
        box().name("X").
            box().name("A").
                newPort("a").
                newPort("b").
                newPort("c").
            endbox().
        endbox().
        box().name("Y").
            box().name("A").
                newPort("b").
                newPort("c").
                newPort("d").
            endbox().
        endbox().
        box().name("C").
            newPort("a").
            newPort("b").
            newPort("c").
            newPort("x").
        endbox().
        box().name("C").
            newPort("d").
            newPort("c").
            newPort("b").
            newPort("y").
        endbox().
        box().name("D").
            newPort("a").
            newPort("e").
            newPort("d").
        endbox().
    endbox();

    Path path("*<Port>");
    QVector<Port*> ports = path.resolveMany<Port>();
    UniqueName unique(ports);
    QStringList names = unique.resolve(), expected;
//    std::cout << qPrintable(names.join("\n"));
    expected
      << "A/a"
      << "X/A/b"
      << "X/A/c"
      << "Y/A/b"
      << "Y/A/c"
      << "A/d"
      << "C/a"
      << "/root/C/b_1"
      << "/root/C/c_1"
      << "x"
      << "C/d"
      << "/root/C/c_2"
      << "/root/C/b_2"
      << "y"
      << "D/a"
      << "e"
      << "D/d";
    QCOMPARE(names, expected);
    delete builder.content();
}

