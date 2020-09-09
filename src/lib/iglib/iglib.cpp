#define IGLIB_DLL dllexport
#include <iostream>
#include <math.h>
#include <string>
#include <QApplication>
#include <QDateTime>
#include <QString>
#include <QTextStream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command.h>
#include <base/convert.h>
#include <base/data_frame.h>
#include <base/dialog_quiet.h>
#include <base/dialog_stub.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/object_pool.h>
#include <base/mega_factory.h>
#include <base/phys_math.h>
#include <base/test_num.h>
#include "iglib.h"

using namespace std;
using namespace base;
using namespace phys_math;
using TestNum::snapToZero;

namespace ig {

// Flags
static bool _initialized = false;
// Global objects
static QCoreApplication *_app = nullptr;
static DialogBase *_dialog = nullptr;
// Buffers
static std::string _queryString, _responseString, _errorString;
static QVector<HeatPipe> _heatPipes;
static QVector<GrowthLight> _growthLights;
static QVector<Vent> _vents;
static QVector<Screen> _screens;

// Functions

void init() {
    const bool test = false;
    static int argc = 1;
    static char ch = 0;
    static char *argv = &ch;

    if (!_initialized) {
        // Create app objcet
        _app = new QCoreApplication(argc, &argv);
        new base::ObjectPool(_app);
        // Configure dialog
        if (test)
            _dialog = new DialogStub(_app);
        else
            _dialog = new DialogQuiet(_app);
        // Only once
        _initialized = true;
    }
}

static double value(Variable v) {
    return (v.origin == NotAvailable) ? -999. : v.value;
}

#define VAR(X) X.value, X.origin!=NotAvailable

void buildCalendar(Box *parent, const Query &q) {
    QDate date = QDate(2001,1,1);
    date = date.addDays(q.timeStamp.dayOfYear-1);
    QTime time = QTime(0,0,0);
    time = time.addSecs(60*60*static_cast<int>(q.timeStamp.timeOfDay));
    QDateTime dateTime = QDateTime(date, time, Qt::UTC);

    BoxBuilder builder(parent);
    builder.
    box("Calendar").name("calendar").
        port("latitude").equals(q.greenhouse.latitude).
        port("longitude").equals(q.greenhouse.longitude).
        port("timeZone").equals(q.timeStamp.timeZone).
        port("initialDateTime").equals(dateTime).
        port("timeStep").equals(30).
        port("timeUnit").equals("s").
        box("Sun").name("sun").
        endbox().
    endbox();
}

void buildSensor(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
    box("vg::Sensor").name("sensor").
        port("indoorsTemperatureIn").equals(value(q.indoors.temperature)).
        port("indoorsRhIn").equals(value(q.indoors.rh)).
        port("indoorsCo2In").equals(value(q.indoors.co2)).
        port("indoorsWindspeedIn").equals(-999.).
        port("outdoorsTemperatureIn").equals(value(q.outdoors.temperature)).
        port("outdoorsRhIn").equals(value(q.outdoors.rh)).
        port("outdoorsCo2In").equals(value(q.outdoors.co2)).
        port("outdoorsGlobalRadiationIn").equals(value(q.outdoors.irradiation)).
        port("outdoorsWindSpeedIn").equals(value(q.outdoors.windspeed)).
        port("soilTemperatureIn").equals(-999.).
    endbox();
}

void buildOutdoors(Box *parent) {
    BoxBuilder builder(parent);
    builder.
    box("vg::Outdoors").name("outdoors").
        port("co2").imports("sensor[outdoorsCo2]").
        port("soilTemperature").imports("sensor[soilTemperature]").
        port("temperature").imports("sensor[outdoorsTemperature]").
        port("rh").imports("sensor[outdoorsRh]").
        port("radiation").imports("sensor[outdoorsGlobalRadiation]").
        port("windSpeed").imports("sensor[outdoorsWindSpeed]").
        port("radiation").imports("sensor[outdoorsGlobalRadiation]").
        port("skyTemperature").imports("./skyTemperatureEstimate[temperature]").
        box("SkyTemperature").name("skyTemperatureEstimate").
            port("airTemperature").imports("sensor[outdoorsTemperature]").
            port("rh").imports("sensor[indoorsRh]").
        endbox().
        box().name("records").
        endbox().
    endbox();
}
Box* buildScreen(const Screen *s) {
    BoxBuilder builder;
    double effect = (s->effect.origin!=NotAvailable) ? s->effect.value/100. : 0.,
           transmissivityLight = s->material.transmissivityLight,
           emmisivityTop = s->material.emmisivityOuter,
           emmisivityBottom = s->material.emmisivityInner,
           transmissivityAir = s->material.transmissivityAir;

    if (transmissivityLight > 1.) {
        dialog().information("WARNING: screen transmissivityLight divided by 100 !!");
        transmissivityLight /= 100.;
    }
    if (emmisivityTop > 1.) {
        dialog().information("WARNING: screen emmisivityOuter divided by 100 !!");
        emmisivityTop /= 100.;
    }
    if (emmisivityBottom > 1.) {
        dialog().information("WARNING: screen emmisivityInner divided by 100 !!");
        emmisivityBottom /= 100.;
    }
    if (transmissivityAir > 1.) {
        dialog().information("WARNING: screen transmissivityLight divided by 100 !!");
        transmissivityAir /= 100.;
    }

    bool isRoof = (s->position==WholeRoof ||  s->position==FlatRoof ||  s->position==Roof1 || s->position==Roof2);
    QString screenClass = isRoof ? "vg::ScreenRoof" : "vg::ScreenWall";
    builder.
        box(screenClass).name("screen").
            port("swReflectivityTop").equals(1. - emmisivityTop).
            port("swReflectivityBottom").equals(1. - emmisivityBottom).
            port("swTransmissivityTop").equals(transmissivityLight).
            port("swTransmissivityBottom").equals(transmissivityLight).
            port("lwReflectivityTop").equals(1. - emmisivityTop).
            port("lwReflectivityBottom").equals(1. - emmisivityBottom).
            port("lwTransmissivityTop").equals(transmissivityLight).
            port("lwTransmissivityBottom").equals(transmissivityLight).
//            port("U").equals(s->material.U).  // don't trust this
//            port("specificHeatCapacity").equals(s->material.heatCapacity).  // don't trust this
            port("transmissivityAir").equals(transmissivityAir).
            port("state").equals(effect).
        endbox();
    return builder.content(BoxBuilder::AmendNone);
}

Box* buildScreens(Screens screens, ScreenPosition position) {
    QVector<const Screen*> selected;
    for (int i=0; i < screens.size; ++i) {
        ScreenPosition pos = screens.array[i].position;
        bool match =
                (pos==position) ||
                ((pos==WholeRoof || pos==FlatRoof) && (position==Roof1 || position==Roof2));
        if (match)
            selected << &screens.array[i];
    }

    BoxBuilder builder;
    builder.box("vg::Screens").name("screens");
    for (const Screen *screen : selected) {
        builder.box(buildScreen(screen));
    }
    builder.endbox();
    return builder.content(BoxBuilder::AmendNone);
}

// Ignore cover parameters from IG user, until they have been checked
#define SHELTERFACE(X, Y, N) \
box("vg::ShelterFace").name(#X). \
    box("vg::ShelterFaceArea").name("area"). \
    endbox(). \
    box("vg::Cover").name("cover"). \
    endbox(). \
    box(buildScreens(q.screens, Y))

void buildConstruction(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
    box().name("construction").
        box("vg::Geometry").name("geometry").
            port("numSpans").equals(q.construction.spanCount).
            port("spanWidth").equals(q.construction.spanWidth).
            port("length").equals(q.construction.length).
            port("height").equals(q.construction.wallHeight).
            port("roofPitch").equals(q.construction.roofInclination).
            port("reflection").equals(q.construction.internalShading).
        endbox().
        box("vg::Shelter").name("shelter").
            SHELTERFACE(roof1, Roof1, 0).endbox().
            SHELTERFACE(roof2, Roof2, 1).endbox().
            SHELTERFACE(side1, Side1, 2).endbox().
            SHELTERFACE(side2, Side2, 3).endbox().
            SHELTERFACE(end1, End1, 4).endbox().
            SHELTERFACE(end2, End2, 5).endbox().
        endbox().
    endbox();
}

//void buildEnergyBudget(Box *parent, const Query &q);
//void buildWaterBudget(Box *parent, const Query &q);
//void buildIndoors(Box *parent, const Query &q);
//void buildCrop(Box *parent, const Query &q);

Box* build(const Query &q) {
    init();
    BoxBuilder builder;
    Box *sim(nullptr);
    environment().option("dontAutoCreateRecords", true);
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(5).
            endbox();
        sim = builder.content();
        buildCalendar(sim, q);
        buildSensor(sim, q);
        buildOutdoors(sim);
        buildConstruction(sim, q);
//        buildEnergyBudget(sim, q);
//        buildIndoors(sim, q);
//        buildWaterBudget(sim, q);
//        buildCrop(sim, q);
    }
    catch (Exception &ex) {
        std::cout << "EXCEPTION\n" << qPrintable(ex.what()) << "\n";
    }
    environment().root(sim);
    return sim;
}

Response testConstant(const Query &q) {
    Response r;
    r.timeStamp = q.timeStamp;
    r.indoorsCo2 = 3.5;
    r.indoorsRh = 7.11;
    r.indoorsTemperature = 13.17;
    r.growthLight = 19.23;
    r.heating = 29.31;
    r.photosynthesis = 37.41;
    r.costEfficiency = 43.47;
    return r;
}

Response testMultiplum(const Query &q) {
    Response r;
    r.timeStamp = q.timeStamp;
    r.indoorsCo2 = r.timeStamp.dayOfYear*r.timeStamp.timeOfDay;
    r.indoorsRh = r.timeStamp.timeOfDay*r.timeStamp.timeZone;
    return r;
}

Response compute(const Query &q) {
    const bool debug = false;
    const bool run = true;

    init();
    Response r;
    bool excepted(false);

    // Build model from root, (write script) and run
    Box *root(nullptr);
    try {
        root = build(q);
        if (debug) {
            environment().latestLoadArg("igclient.box");
            Command::submit(QStringList() << "write", nullptr);
        }
        if (run) {
            root->run();
        }
    }
    catch (Exception &ex) {
        _errorString = ex.what().toStdString();
        cout << "\n\nERROR: " << _errorString << "\n";
        excepted = true;
    }

    // Check for error
    if (excepted || root->port("hasError")->value<bool>()) {
        if (!excepted)
            _errorString = root->port("errorMsg")->value<QString>().toStdString();
        r.hasError = true;
        r.error = _errorString.c_str();
    }

    // Extract response from model state
    try {
        r.timeStamp = q.timeStamp;
        r.indoorsCo2 = root->findOne<Box>("indoors/co2")->port("value")->value<double>();
        r.indoorsRh = root->findOne<Box>("indoors/humidity")->port("rh")->value<double>();
        r.indoorsTemperature = root->findOne<Box>("indoors/temperature")->port("value")->value<double>();
        r.indoorsPar = root->findOne<Box>("indoors/light")->port("parTotal")->value<double>();
        snapToZero(r.indoorsPar);
        QVector<Box*> pipes = root->findMany<Box>("pipes/pipe");
        r.heating = 0; for (Box *pipe : pipes) r.heating += pipe->port("effect")->value<double>();
        snapToZero(r.heating);
        r.photosynthesis = root->findOne<Box>("crop/Pg")->port("value")->value<double>();
        snapToZero(r.photosynthesis);
        r.growthLight = root->findOne<Box>("actuators/growthLights")->port("powerUsage")->value<double>();
        snapToZero(r.growthLight);
        double E = r.heating + r.growthLight;
        snapToZero(E);
        r.costEfficiency = (E==0.) ? 0. : r.photosynthesis/E*1000./3600.;
    }
    catch (Exception &ex) {
        std::cout << ex.what().toStdString() << "\n";
    }

    return r;
}

#define RESP(X) text << #X << " = " << r.X << "\n"

const char * responseToString(const Response &r) {
    QString s;
    QTextStream text(&s);
    RESP(timeStamp.dayOfYear);
    RESP(timeStamp.timeOfDay);
    RESP(timeStamp.timeZone);
    RESP(indoorsCo2);
    RESP(indoorsRh);
    RESP(indoorsTemperature);
    RESP(indoorsPar);
    RESP(growthLight);
    RESP(heating);
    RESP(photosynthesis);
    RESP(costEfficiency);
    if (r.hasError)
        text << "ERROR:\n" << qPrintable(QString::fromStdString(r.error)) << "\n\n";
    else
        text << "No errors\n";
    _responseString = text.string()->toStdString();
    return _responseString.c_str();
}

} // namespace
