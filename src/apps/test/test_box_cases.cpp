#include <boxes/box.h>
#include <boxes/box_builder.h>
#include <boxes/exception.h>
#include "test_box_cases.h"

using namespace boxes;

namespace TestBoxCases {

    QString Av1, Av3,A1v1;
    int sum, sum2;

    Box* case1() {
        BoxBuilder builder;
        builder.
            box().name("A").
                newPort("v1").data(&Av1).equals(QString("ape")).
                newPort("v3").data(&Av3).equals("monkey").
                box().name("A1").
                    newPort("v1").data(&A1v1).equals(100).
                    box().name("a").
                        newPort("v1").
                    endbox().
                    box().name("a").
                        newPort("v1").
                    endbox().
                    box().name("c").
                        newPort("v2").
                    endbox().
                endbox().
                box().name("A2").
                    newPort("v2").
                    box().name("a").endbox().
                    box().name("b").
                        newPort("v1").
                    endbox().
                    box().name("c").
                        newPort("v2").
                    endbox().
                endbox().
                box().name("A3").
                    newPort("v3").
                    box().name("a").
                        newPort("v2").
                    endbox().
                    box().name("b").
                        newPort("v3").
                    endbox().
                endbox().
            endbox();
        return builder.content();
    }

    Box* case2() {
        BoxBuilder builder;
        builder.
            box("Simulation").name("savanna").
                port("steps").equals(2).
                box().name("lion").
                    newPort("sum").data(&sum).equals(77).
                    newPort("sum2").data(&sum2).import("./juvenile[n]").import("./adult[n]").transform(Sum).
                    box("Lion").name("juvenile").
                        port("n").equals(25).
                    endbox().
                    box("Lion").name("adult").
                        port("n").equals(7).
                        port("preyDensity").import("zebra[density]").import("gnu[density]").transform(Sum).
                    endbox().
                endbox().
                box("Grazer").name("zebra").
                    port("density").equals(100).
                    port("killRate").import("lion/adult[killRate]").
                endbox().
                box("Grazer").name("gnu").
                port("density").equals(20).
                    port("killRate").import("lion/adult[killRate]").
                endbox().
            endbox();
        return builder.content();
    }

} // namespace

