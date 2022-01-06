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

void buildScreen(Box *parent, const Screen *s) {
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

    bool isRoof = (s->position==Roof1 || s->position==Roof2);
    QString screenClass = isRoof ? "vg::ScreenRoof" : "vg::ScreenWall";

    double reflectivityTop    = 1. - emmisivityTop    - transmissivityLight,
           reflectivityBottom = 1. - emmisivityBottom - transmissivityLight;
    TestNum::snapToZero(reflectivityTop);
    TestNum::snapTo(reflectivityTop, 1.);
    TestNum::snapToZero(reflectivityBottom);
    TestNum::snapTo(reflectivityBottom, 1.);

    BoxBuilder builder(parent);
    builder.
    box(screenClass).name("screen").
        port("swReflectivityTop").equals(reflectivityTop).
        port("swReflectivityBottom").equals(reflectivityBottom).
        port("swTransmissivityTop").equals(transmissivityLight).
        port("swTransmissivityBottom").equals(transmissivityLight).
        port("lwReflectivityTop").equals(reflectivityTop).
        port("lwReflectivityBottom").equals(reflectivityBottom).
        port("lwTransmissivityTop").equals(transmissivityLight).
        port("lwTransmissivityBottom").equals(transmissivityLight).
        port("transmissivityAir").equals(transmissivityAir).
        port("ventilation").equals(0.).
        port("state").equals(effect).
    endbox();
}

QString toString(ScreenPosition pos) {
    switch(pos) {
    case Roof1: return "roof1";
    case Roof2: return "roof2";
    case Side1: return "side1";
    case Side2: return "side2";
    case End1: return "end1";
    case End2: return "end2";
    }
    return "";
}

void buildScreens(Box *parent, ScreenPosition pos, const Query &q) {
    for (int i=0; i < q.screens.size; ++i) {
        ScreenPosition screenPos = q.screens.array[i].position;
        if (screenPos == pos)
            buildScreen(parent, &q.screens.array[i]);
    }
}

void buildShelterFace(Box *parent, ScreenPosition pos, const Query &q) {
    QString faceName = toString(pos);

    // Ignore cover parameters from IG user until they have been checked
    BoxBuilder builder(parent);
    builder.
    box("vg::ShelterFace").name(faceName).
        box("vg::ShelterFaceArea").name("area").
        endbox().
        box("vg::Cover").name("cover").
        endbox().
        box("vg::Screens").name("screens").
        endbox().
    endbox();
    Box *screens = parent->findOne<Box*>(faceName + "/screens");
    buildScreens(screens, pos, q);
}

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
        endbox().
        box().name("floor").
            newPort("reflectivity").equals(0.6).
            newPort("Utop").equals(7.5).
            newPort("Ubottom").equals(0.1).
            newPort("heatCapacity").equals(42000.).
        endbox().
    endbox();

    Box *shelter = parent->findOne<Box*>("shelter");
    buildShelterFace(shelter, Roof1, q);
    buildShelterFace(shelter, Roof2, q);
    buildShelterFace(shelter, Side1, q);
    buildShelterFace(shelter, Side2, q);
    buildShelterFace(shelter, End1, q);
    buildShelterFace(shelter, End2, q);
}

void buildPipe(Box *parent, const HeatPipe *pipe) {
    BoxBuilder builder(parent);
    builder.
        box("vg::PipeForced").name("pipe").
            port("Tair").imports("sensor[indoorsTemperature]").
            port("innerDiameter").equals(pipe->innerDiameter).
            port("waterVolume").equals(pipe->waterVolume).
            port("flowRate").equals(value(pipe->flowRate)).
            port("Tinflow").equals(value(pipe->temperatureInflow)).
            port("knownToutflow").equals(value(pipe->temperatureOutflow)).
        endbox();}

void buildPipes(Box *parent, const HeatPipes pipes) {
    BoxBuilder builder(parent);
    builder.
        box("ActuatorHeatPipes").name("heating").
            box().name("pipes").
            endbox().
        endbox();
    Box *pipesBox =  parent->findOne<Box*>("pipes");
    for (int i=0; i < pipes.size; ++i)
        buildPipe(pipesBox, &pipes.array[i]);
}

void buildGrowthLight(Box *parent, const GrowthLight *g) {
    double propLw = (g->type == Led) ? 0.1 : 0.5;
    BoxBuilder builder(parent);
    builder.
        box("vg::GrowthLight").name("growthLight").
            port("parPhotonCoef").equals(g->parEfficiency).
            port("propLw").equals(propLw).
            port("intensity").equals(g->powerUsage).
            port("ageCorrectedEfficiency").equals(g->ageCorrectedEfficiency).
            port("on").equals(g->powerUsage>0.).
        endbox();
}

void buildGrowthLights(Box *parent, GrowthLights lights) {
    BoxBuilder builder(parent);
    builder.
        box("vg::GrowthLights").name("growthLights").
        endbox();
    Box *growthLights =  parent->findChild<Box*>("growthLights");
    for (int i=0; i < lights.size; ++i)
        buildGrowthLight(growthLights, &lights.array[i]);
}

void buildActuators(Box *parent, const Query &q) {
    // Use maximum opening value available
    double opening = 0;
    for (int i=0; i<q.vents.size; ++i) {
        Variable var = q.vents.array[i].opening;
        if (var.origin!=NotAvailable && var.value > opening)
            opening = std::min(var.value, 1.);
    }

    BoxBuilder builder(parent);
    builder.
    box("Actuators").name("actuators").
            box("ActuatorVentilation").name("ventilation").
                port("minFlux").equals(0.).
                port("indoorsTemperature").imports("sensor[indoorsTemperature]").
            endbox().
    endbox();
    Box *actuators = parent->findChild<Box*>("actuators");
    buildPipes(actuators, q.heatPipes);
    buildGrowthLights(actuators, q.growthLights);
}

void buildEnergyBudget(Box *parent) {
    BoxBuilder builder(parent);
    builder.
    box("vg::EnergyBudget").name("energyBudget").
    endbox();
}

void buildEnergyBudgetIndoors(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
    box().name("indoors").
        box("vg::IndoorsVentilation").name("ventilation").
            box("vg::LeakageVentilation").name("leakage").
                port("leakage").equals(q.construction.infiltration).
            endbox().
        endbox().
        box("vg::IndoorsTemperature").name("temperature").
        endbox().
    endbox();
}

void buildWaterBudget(Box *parent) {
    BoxBuilder builder(parent);
    builder.
    box("vg::WaterBudget").name("waterBudget").
//        box().name("condensationScreens").
//            box().name("screen").
//                newPort("conductance").equals(0.).
//                newPort("vapourFlux").equals(0.).
//                newPort("gain").equals(0.).
//            endbox().
//        endbox().
    endbox();
}

void buildIndoors(Box *parent, const Query &q) {
    Variable var = q.indoors.co2;
    double co2 = (var.origin!=NotAvailable) ? var.value : 900.;
    if (TestNum::eqZero(co2))
        co2 = 900.;
    BoxBuilder builder(parent);
    builder.
    box("vg::Indoors").name("indoors").
        box("vg::IndoorsCo2").name("co2").
            port("outdoorsCo2").equals(co2).
            port("airFlux").equals(0.).
            port("injectionRate").equals(0.).
            port("timeStep").equals(0.).
        endbox().
    endbox();
}

void buildCrop(Box *parent) {
    BoxBuilder builder(parent);
    builder.
    box("vg::Crop").name("crop").
    endbox();
}


Box* build(const Query &q) {
    init();
    BoxBuilder builder;
    Box *sim(nullptr);
    environment().option("dontAutoCreateRecords", true);
    try {
        builder.
            box("Simulation").name("greenhouse").
                port("steps").equals(5).
            endbox();
        sim = builder.content();
        buildCalendar(sim, q);
        buildSensor(sim, q);
        buildOutdoors(sim);
        buildConstruction(sim, q);
        buildActuators(sim, q);
        buildEnergyBudget(sim);
        buildEnergyBudgetIndoors(sim->findOne<Box*>("energyBudget"), q);
        buildWaterBudget(sim);
        buildIndoors(sim, q);
        buildCrop(sim);
    }
    catch (Exception &ex) {
        std::cout << "EXCEPTION\n" << qPrintable(ex.what()) << "\n";
    }
    environment().root(sim);
//    Box *crop = sim->findOne<Box*>("greenhouse/crop");
//    std::cout << qPrintable(crop->name()) << "\n";
//    QVector<Box*> children = crop->findMany<Box*>("./*");
//    std::cout << children.size() << " children\n";
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
    bool excepted(false);
    Response r;

    init();

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
        r.indoorsCo2 = root->findOne<Box*>("indoors/co2")->port("value")->value<double>();
        r.indoorsRh = root->findOne<Box*>("indoors/humidity")->port("rh")->value<double>();
        r.indoorsTemperature = root->findOne<Box*>("indoors/temperature")->port("value")->value<double>();
        r.indoorsPar = root->findOne<Box*>("energyBudget")->port("cropParFluxFromAbove")->value<double>();
        snapToZero(r.indoorsPar);
        r.heating = root->findOne<Box*>("actuators/heating")->port("energyFluxTotal")->value<double>();
        snapToZero(r.heating);
        r.photosynthesis = root->findOne<Box*>("crop/photosynthesis")->port("Pg")->value<double>();
        snapToZero(r.photosynthesis);
        r.growthLight = root->findOne<Box*>("actuators/growthLights")->port("powerUsage")->value<double>();
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

Response blankResponse() {
    Response r;
    r.indoorsCo2 = 100.1;
    r.indoorsRh = 100.2;
    r.indoorsTemperature = 100.3;
    r.indoorsPar = 100.4;
    r.growthLight = 100.5;
    r.heating = 100.6;
    r.photosynthesis = 100.7;
    r.costEfficiency = 100.8;
    r.grayMoldRisk = 100.9;
    r.daysToHarvest = 100.10;
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
