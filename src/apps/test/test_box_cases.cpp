#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include "test_box_cases.h"

using namespace base;

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
                    newPort("sum2").data(&sum2).imports("./juvenile[n]").imports("./adult[n]").transform(Sum).
                    box("Lion").name("juvenile").
                        port("n").equals(25).
                    endbox().
                    box("Lion").name("adult").
                        port("n").equals(7).
                        port("preyDensity").imports("zebra[density]").imports("gnu[density]").transform(Sum).
                    endbox().
                endbox().
                box("Grazer").name("zebra").
                    port("initialDensity").equals(100).
                    port("killRate").imports("lion/adult[killRate]").
                endbox().
                box("Grazer").name("gnu").
                port("initialDensity").equals(20).
                    port("killRate").imports("lion/adult[killRate]").
                endbox().
            endbox();
        return builder.content();
    }

    Box* case3a() {
        BoxBuilder builder;
        builder.
            box("Simulation").name("test_box_cases_3a").
                port("steps").equals(2).
                box("ModelA").name("A").
                    port("input2").trackOn().
                    port("input4").trackOn().
                    port("output2").trackOff().
                    port("output4").trackOff().
                endbox().
            endbox();
        return builder.content();
    }

} // namespace

