#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/path.h>
#include <base/port.h>
#include <base/unique_name.h>
#include "test_unique_name.h"

using namespace base;

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
            newPort("x").label("y").
        endbox().
        box().name("D").
            newPort("a").
            newPort("e").
            newPort("d").
        endbox().
    endbox();

    Path path("*{Port}");
    QVector<Port*> ports = path.resolve<Port>();
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

void TestUniqueName::testSandy() {
    BoxBuilder builder;
    builder.
    box("Simulation").name("sandy_test").
        port("steps").equals(100).
        port("step").
        box("Sequence").name("seq").
            port("min").equals(10).
            port("max").equals(30).
            port("value").label("x").
        endbox().
        box("Sandy").name("A").
            port("x").imports("seq[value]").
            port("a").equals(0).
            port("b").equals(0).
            port("value").label("A").
        endbox().
        box("Sandy").name("B").
            port("x").imports("seq[value]").
            port("a").equals(0.5).
            port("b").equals(0.5).
            port("value").label("B").
        endbox().
        box("Sandy").name("C").
            port("x").imports("seq[value]").
            port("a").equals(1).
            port("b").equals(1).
        endbox().
        box("Sandy").name("D").
            port("x").imports("seq[value]").
            port("a").equals(5).
            port("b").equals(5).
        endbox().
        box("Sandy").name("E").
            port("x").imports("seq[value]").
            port("a").equals(2).
            port("b").equals(1).
        endbox().
        box("Sandy").name("F").
            port("x").imports("seq[value]").
            port("a").equals(4).
            port("b").equals(0.5).
        endbox().
    endbox();

    Path path("*{Port}");
    QVector<Port*> ports = path.resolve<Port>();
    UniqueName unique(ports);
    QStringList names = unique.resolve(), expected;
    std::cout << qPrintable(names.join("\n"));
    delete builder.content();
}
