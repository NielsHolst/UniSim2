#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include "test_box_cases.h"

using namespace base;
using namespace std;

namespace TestBoxCases {

    static int sum2;

    Box* case1() {
        BoxBuilder builder;
        builder.
            box().name("A").
                newPort("v1").equals(QString("ape")).
                newPort("v3").equals("monkey").
                box().name("A1").
                    newPort("v1").equals(100).
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
                    newPort("sum").equals(77).track().
                    newPort("sum2").data(&sum2).imports("./*<Lion>[n]").track().transform(Sum).
                    box("Lion").name("juvenile").
                        port("n").equals(25).
                    endbox().
                    box("Lion").name("adult").
                        port("n").equals(7).
                        port("preyDensity").imports("*<Grazer>[density]").transform(Sum).
                    endbox().
                endbox().
                box("Grazer").name("zebra").
                    port("initialDensity").equals(100).
                    port("killRate").imports("lion/adult[killRate]").track().
                endbox().
                box("Grazer").name("gnu").
                port("initialDensity").equals(20).
                    port("killRate").imports("../lion/adult[killRate]").
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
                    port("input2").track().
                    port("input4").track().
                    port("output1").track().
                    port("output3").track().
                endbox().
                box("OutputR").
                endbox().
            endbox();
        return builder.content();
    }

    Box* case4() {
        BoxBuilder builder;
        builder.
            box("Simulation").name("test_box_cases_4").
                port("steps").equals(2).
                box("test_boxes::VectorInput").name("V").
                    port("numbers1").equals("7").
                    port("numbers2").equals("(7 9 13)").
                endbox().
                box("OutputR").name("output").
                    box("PageR").name("page").
                        box("PlotR").name("plot").
                            port("ports").equals("(V[*])").
                        endbox().
                    endbox().
                endbox().
            endbox();
        return builder.content();
    }

    Box* case5a() {
        BoxBuilder builder;
        try {
            builder.
                box("Simulation").name("test_box_cases_3a").
                    port("steps").equals(2).
                    box("ModelA").name("A").
                        port("input2").imports(".[input1]").
                    endbox().
                    box("OutputR").
                    endbox().
                endbox();
        }
        catch(Exception &ex) {
            QString s = "Unexpected exception: " + ex.what() + "\n";
            cout << (qPrintable(s));
        }
        return builder.content();
    }

    Box* case5b() {
        BoxBuilder builder;
        builder.
            box("Simulation").name("test_box_cases_3a").
                port("steps").equals(2).
                box("ModelA").name("parent").
                    box("ModelA").name("A").
                        port("input1").imports("parent[input1]").
                    endbox().
                endbox().
                box("OutputR").
                endbox().
            endbox();
        return builder.content();
    }
} // namespace

