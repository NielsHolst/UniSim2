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
            box("A").
                port("v1").data(&Av1).equals(QString("ape")).
                port("v3").data(&Av3).equals("monkey").
                box("A1").
                    port("v1").data(&A1v1).equals(100).
                    box("a").
                        port("v1").
                    endbox().
                    box("a").
                        port("v1").
                    endbox().
                    box("c").
                        port("v2").
                    endbox().
                endbox().
                box("A2").
                    port("v2").
                    box("a").endbox().
                    box("b").
                        port("v1").
                    endbox().
                    box("c").
                        port("v2").
                    endbox().
                endbox().
                box("A3").
                    port("v3").
                    box("a").
                        port("v2").
                    endbox().
                    box("b").
                        port("v3").
                    endbox().
                endbox().
            endbox();
        return builder.content();
    }

}

