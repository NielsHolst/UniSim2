#include <base/box.h>
#include <base/box_builder.h>
#include <base/test_num.h>
#include "test_weather.h"

using namespace base;


void TestPortWeather::testOk() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            box("boxes::Weather").name("weather").
                box().name("records").
                    newPort("Tmin").equals(20.1).
                    newPort("Tmax").equals(20.9).
                endbox().
            endbox().
        endbox();
    Box *sim = builder.content();
    try {
        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    double Tavg = sim->findOne<Port>("weather[Tavg]")->value<double>();
    QVERIFY(TestNum::eq(Tavg, 20.5));
    delete sim;
}

