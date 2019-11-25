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
#include <base/phys_math.h>
#include <base/mega_factory.h>
#include "iglib.h"

using namespace std;
using namespace base;
using namespace phys_math;

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

QString toString(ScreenLayer layer) {
    switch(layer) {
    case Inner: return "Inner";
    case Mid: return "Mid";
    case Outer: return "Outer";
    }
    return QString();
}

QString toString(ScreenPosition position) {
    switch(position) {
    case WholeRoof: return "roof1_roof2";
    case FlatRoof: return "roof1_roof2_horizontal";
    case End1: return "end1";
    case End2:  return "end2";
    case Side1: return "side1";
    case Side2: return "side2";
    case Roof1: return "roof1";
    case Roof2: return "roof2";
    }
    return QString();
}

QString toString(GrowthLightType type) {
    switch(type) {
    case Hpsl: return "HPSL";
    case Led: return "LED";
    }
    return QString();
}

Box* buildControllers(const Query &) {
    BoxBuilder builder;
    builder.
        box().name("controllers").
            box().name("chalk").
                newPort("value").equals(0).
            endbox().
            box().name("co2").
                newPort("signal").equals(0).
            endbox().
        endbox();
    return builder.content(BoxBuilder::AmendNone);
}

Box* buildControlled() {
    BoxBuilder builder;
    builder.
        box().name("controlled").
            box().name("energyFlux").
                box().name("heating").
                    box("Sum").name("minSupply").
                        port("inputs").imports("actuators/heating/pipes/*[nextEnergyFluxMin]").
                    endbox().
                    box("Sum").name("maxSupply").
                        port("inputs").imports("actuators/heating/pipes/*[nextEnergyFluxMax]").
                    endbox().
                    box("EnergyFluxHeatingDemand").name("demand").
                    endbox().
                    box("PidController").name("supply").
                        port("Kprop").equals(0.6).
                        port("Kint").equals(0.01).
                        port("minimum").imports("../minSupply[value]").
                        port("maximum").imports("../maxSupply[value]").
                        port("desiredValue").imports("../demand[value]").
                        port("sensedValue").imports("actuators/heating/pipes/*<Pipe>[energyFlux]").transform(Sum).
                    endbox().
                endbox().
                box("IndoorsTemperature").name("temperature").
                    port("energyFlux").imports("./energyFlux[value]").
                    port("baseTemperature").imports("indoors/temperature[value]").
                    box("Sum").name("energyFlux").
                        port("inputs").equals("(given/energyFlux[value] energyFlux/heating/supply[value])"). // check Sum inputs type!!
                    endbox().
                endbox().
                box().name("cooling").
                endbox().
            endbox().
            box().name("cooling").
            endbox().
        endbox();
    return builder.content(BoxBuilder::AmendNone);
}

void buildGrowthLight(Box *parent, const GrowthLight *g) {
    BoxBuilder builder(parent);
    builder.
        box("vg::GrowthLight").name("growthLight").
            port("type").equals(toString(g->type)).
            port("intensity").equals(g->lightintensity).
            port("parPhotonCoef").equals(g->parEfficiency).
            port("powerUsage").equals(g->powerUsage).
            port("ageCorrectedEfficiency").equals(g->ageCorrectedEfficiency).
            port("on").equals(g->lightintensity>0.).
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

void buildPipe(Box *parent, const HeatPipe *pipe) {
    QString material;
    switch (pipe->material) {
        case Iron:      material = "carbon steel"; break;
        case Plastic:   material = "polyethylene"; break;
        case Copper:    material = "copper"; break;
        case Aluminium: material = "aluminium";
    }

    BoxBuilder builder(parent);
    builder.
        box("vg::Pipe").name("pipe").
            port("material").equals(material).
            port("density").equals(value(pipe->length)).
            port("diameter").equals(value(pipe->innerDiameter)).
            port("flowRate").equals(value(pipe->flowRate)).
        endbox();
}

void buildActuators(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
        box("Actuators").name("actuators").
            box().name("heating").
                newPort("value").imports("sensor[indoorsTemperature]").
            endbox().
            box().name("screens").
            endbox().
            box().name("vents").
            endbox().
        endbox();
    Box *actuators = parent->findChild<Box*>("actuators");
    buildGrowthLights(actuators, q.growthLights);
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
            port("diffuseRadiation").imports("./diffuseIrradiationEstimate[value]").
            port("skyTemperature").imports("./skyTemperatureEstimate[temperature]").
            box("DiffuseIrradiationRE").name("diffuseIrradiationEstimate").
                port("globalIrradiation").imports("sensor[outdoorsGlobalRadiation]").
            endbox().
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
    QString orientation = (s->position==WholeRoof) ? "horizontal" : "cover";
    double effect = (s->effect.origin!=NotAvailable) ? s->effect.value/100. : 0.,
           transmissivityLight = s->material.transmissivityLight,
           emmisivityInner = s->material.emmisivityInner,
           emmisivityOuter = s->material.emmisivityOuter,
           haze = s->material.haze,
           transmissivityAir = s->material.transmissivityAir;

    if (transmissivityLight > 1.) {
        dialog().information("WARNING: screen transmissivityLight divided by 100 !!");
        transmissivityLight /= 100.;
    }
    if (emmisivityInner > 1.) {
        dialog().information("WARNING: screen emmisivityInner divided by 100 !!");
        emmisivityInner /= 100.;
    }
    if (emmisivityOuter > 1.) {
        dialog().information("WARNING: screen emmisivityOuter divided by 100 !!");
        emmisivityOuter /= 100.;
    }
    if (haze > 1.) {
        dialog().information("WARNING: screen haze divided by 100 !!");
        haze /= 100.;
    }
    if (transmissivityAir > 1.) {
        dialog().information("WARNING: screen transmissivityLight divided by 100 !!");
        transmissivityAir /= 100.;
    }

    builder.
        box("vg::Screen").name("screen").
            port("orientation").equals(orientation).
            port("transmissivityLight").equals(transmissivityLight).
            port("emissivityInner").equals(emmisivityInner).
            port("emissivityOuter").equals(emmisivityOuter).
//            port("U").equals(s->material.U).  // don't trust this
//            port("acceptZeroU").equals(true).
            port("energyLossReduction").equals(s->material.energySaving/100.).
            port("haze").equals(haze).
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

Box* buildVent(const Vents &vents, int i) {
    BoxBuilder builder;
    if (i < vents.size) {
        const Vent &v(vents.array[i]);
        double opening = (v.opening.origin!=NotAvailable) ? v.opening.value : 0.;
        builder.
            box("vg::Vent").name("vent").
                port("length").equals(v.length).
                port("width").equals(v.height).
                port("number").equals(v.numberOfVents).
                port("transmissivity").equals(v.porosity).
                port("state").equals(opening).
            endbox();
    }
    else {
        builder.box().name("noVent").endbox();
    }
    return builder.content(BoxBuilder::AmendNone);
}

#define SHELTERFACE(X, Y, N) \
box("vg::ShelterFace").name(#X). \
    box("vg::Cover").name("cover"). \
    endbox(). \
    box(buildScreens(q.screens, Y)). \
    box(buildVent(q.vents, N))
// Ignore cover parameters from IG user, until they have been checked
//        port("U4").equals(q.construction.X.U).
//        port("emissivity").equals(q.construction.X.emissivity).
//        port("absorptivity").equals(q.construction.X.absorptivity).
//        port("transmissivity").equals(q.construction.X.transmissivity).
//        port("haze").equals(q.construction.X.haze).
//        port("specificHeatCapacity").equals(q.construction.X.heatCapacity).

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

void buildGiven(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
        box().name("given").
            box().name("energyFlux").
                box().name("floor").
                    newPort("emissivity").equals(q.construction.floor.emissivity).
                    newPort("temperature").imports("sensor[indoorsTemperature]").
                endbox().
                box().name("shelter").
                    newPort("coverTemperature").imports("sensor[outdoorsTemperature]").
                    newPort("screensTemperature").imports("sensor[indoorsTemperature]").
                endbox().
            endbox().
        endbox();
}

void buildIndoors(Box *parent) {
    BoxBuilder builder(parent);
    builder.
        box("Box").name("indoors").
            box("vg::IndoorsLight").name("light").
            endbox().
            box().name("temperature").
                newPort("value").imports("sensor[indoorsTemperature]").
            endbox().
            box("IgIndoorsHumidity").name("humidity").
                port("rh").imports("sensor[indoorsRh]").
            endbox().
            box().name("windSpeed").
                newPort("value").imports("sensor[indoorsWindspeed]").
            endbox().
            box().name("co2").
                newPort("value").imports("sensor[indoorsCo2]").
            endbox().
        endbox();
}

void buildControllers(Box *parent) {
    BoxBuilder builder(parent);
    builder.
        box().name("controllers").
            newPort("chalk").equals(0.).
        endbox();
}

void buildCrop(Box *parent, const Query &) {
    BoxBuilder builder(parent);
    builder.
        box("vg::Crop").name("crop").
            box().name("lai").
                newPort("value").equals(1.).
                newPort("fractionPlantArea").equals(0.9).
            endbox().
        endbox();
}

//void buildOutput(Box *parent) {
//    BoxBuilder builder(parent);
//    builder.
//        box("OutputBuffer").name("output").
//            port("ports").equals("(crop/layers/*/photosynthesis[Pg]  crop/growth/Pg[value] top/photosynthesis[*] calendar[angot])").
//        endbox();
//}

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
        buildGiven(sim, q);
        buildIndoors(sim);
        buildControllers(sim);
        buildActuators(sim, q);
        buildCrop(sim, q);
//        buildOutput(sim);
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
        r.heating = 0;
        r.costEfficiency = 0;
        r.photosynthesis = root->findOne<Box>("crop/Pg")->port("value")->value<double>();
        r.growthLight = root->findOne<Box>("actuators/growthLights")->port("powerUsage")->value<double>();
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
