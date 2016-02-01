#include <base/box.h>
#include <base/box_builder.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "test.h"

using namespace base;

namespace command {

PUBLISH(test)

test::test(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(test);
}

void test::execute() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("FibSim").
            port("steps").equals(4).
            box("Fibonacci").name("fibonacci").
            endbox().
        endbox();

    Environment &env(environment());
    env.state.root = builder.content();

    DialogBase &dia(dialog());
    dia.information("Loaded FibSim");
}

}
