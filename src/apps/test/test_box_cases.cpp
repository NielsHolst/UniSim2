#include <boxes/box.h>
#include <boxes/box_builder.h>
#include <boxes/exception.h>
#include "test_box_cases.h"

using namespace boxes;

namespace TestBoxCases {

    Box* case1() {
        BoxBuilder builder;
        QString Av1, Av3,A1v1;
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
            box().name("savanna").
                box().name("lion").
                    port("steps").equals(2).
                    box("Lion").name("juvenile").endbox().
                    box("Lion").name("adult").
                        port("preyDensity").import("zebra[density]").
                    endbox().
                endbox().
                box("Grazer").name("zebra").
                    port("killRate").import("lion/adult[killRate]").
                endbox().
            endbox();
        return builder.content();
    }

} // namespace

