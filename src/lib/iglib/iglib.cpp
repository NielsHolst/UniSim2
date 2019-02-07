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
#include <base/dialog_quiet.h>
#include <base/dialog_stub.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/mega_factory.h>
#include "iglib.h"

using namespace base;
using namespace phys_math;

namespace ig {

// Flags
static bool _initialized = false;
// Global objects
static QCoreApplication *_app = nullptr;
static DialogBase *_dialog = nullptr;
static Box *_rnd;
// Buffers
static std::string _queryString, _responseString, _errorString;
static QVector<HeatPipe> _heatPipes;
static QVector<GrowthLight> _growthLights;
static QVector<Vent> _vents;
static QVector<Screen> _screens;

void init() {
    static int argc = 1;
    static char ch = 0;
    static char *argv = &ch;
    if (!_initialized) {
        _app = new QCoreApplication(argc, &argv);
//        _dialog = new DialogQuiet(_app);
        _dialog = new DialogStub(_app); // TEST

        _rnd = MegaFactory::create<Box>("RandomUniform", "rnd", _dialog);
        _rnd->port("drawAtUpdate")->equals(true);
        _rnd->initialize();

        _initialized = true;
    }
}

double rnd(double floor, double ceiling) {
    _rnd->update();
    double value = _rnd->port("value")->value<double>();
    value = floor + value*(ceiling - floor);
    return std::floor(value*100)/100.;
}

int rndi(int floor, int ceiling) {
    return static_cast<int>(rnd(floor,ceiling));
}

Origin rndo() {
    return static_cast<Origin>(rndi(0,3));
}

double value(Variable v) {
    return (v.origin==NotAvailable) ? 0 : v.value;
}

Variable rndv(double floor, double ceiling) {
    double value = rnd(floor,ceiling);
    Origin origin = rndo();
    return Variable{value, origin};
}

QDateTime randomDateTime() {
    return QDateTime(QDate(2000,rndi(1,13),rndi(1,29)), QTime(rndi(0,24),rndi(0,59),0), Qt::UTC);
}

CoverMaterial randomCoverMaterial() {
    CoverMaterial c;
    c.emissivity = rnd(0.8,0.9);
    double a = rnd(0.02, 0.06),
           r = rnd(0.05, 0.1);
    c.absorptivity = a;
    c.transmissivity = 1.-a-r;
    c.haze = rnd(0, 0.3);
    c.U = rnd(5,9);
    c.heatCapacity = rndi(700,900);
    return c;
}

HeatPipe randomHeatPipe() {
    HeatPipe h;
    h.flowRate = rndv(2,20);
    double t = rnd(30,70);
    h.temperatureInflow = Variable{t, rndo()};
    h.temperatureOutflow = Variable{t - rnd(2,8), rndo()};
    double d = rndi(20,28);;
    h.innerDiameter = d - rnd(0.5,1);
    h.outerDiameter = d;
    h.length = rnd(0.5, 1.5);
    return h;
}

Vent randomVent() {
    Vent v;
    v.length = rnd(2,5);
    v.height = rnd(0.5,1);
    v.numberOfVents = rndi(5,10);
    v.maxOpening = rndi(70,120);
    v.porosity = rnd(0.9,1);
    v.opening = Variable{rnd(0,1), Measured};
    return v;
}

GrowthLight randomGrowthLight() {
    GrowthLight g;
    g.type = static_cast<GrowthLightType>(rndi(0,2)) ;
    g.intensity = rndi(20,50);
    g.ballastCorrection = 1 + 0.05*rndi(0,3);
    double v =rnd(1000,10000);
    Origin o = rndo();
    g.age = Variable{v, o};
    g.lifeTime = Variable{v*rnd(5,10), o};
    g.on = Variable{static_cast<double>(rndi(0,2)), Measured};
    return g;
}

Screen randomScreen() {
    Screen s;
    double tr = rnd(0.3,0.6);
    s.material.transmissivityLight = tr;
    s.material.emmisivityInner = rnd(0.1, 0.9-tr);
    s.material.emmisivityOuter = rnd(0.1, 0.9-tr);
    s.material.haze = rnd(0,0.1);
    s.material.energySaving = rnd(0.3,0.6);
    s.material.transmissivityAir = rnd(0.7,0.9);
    s.material.U = rnd(4,8);
    s.material.heatCapacity = rnd(300,700);
    s.layer = static_cast<ScreenLayer>(rndi(0,3));
    s.position =static_cast<ScreenPosition>(rndi(0,8));
    s.effect = Variable{rnd(0,1), Measured};
    return s;
}

#define RANDOM_VECTOR(X,Y) \
_##X.resize(n); \
for (int i=0; i<n; ++i)  _##X[i] = Y(); \
q.X.size = n; \
q.X.array = _##X.data()


Query randomQuery() {
    init();
    Query q;
    try {
        QDateTime dateTime = randomDateTime();
        double latitude = rnd(0,65),
               longitude = rnd(-180,180);
        q.timeStamp.dayOfYear = dateTime.date().dayOfYear();
        q.timeStamp.timeOfDay = dateTime.time().hour() + dateTime.time().minute()/60.;
        q.timeStamp.timeZone = static_cast<int>((longitude/15. + 0.5));
        q.greenhouse.latitude = latitude;
        q.greenhouse.longitude = longitude;
        q.greenhouse.direction = rndi(0,360);
        q.culture.type = static_cast<CultureType>(rndi(0,5));
        q.culture.lai = rndv(1,5);
        q.construction.internalShading = rnd(0.05,0.1);
        q.construction.length = 10*rndi(5,13);
        q.construction.spanWidth = 5*rndi(4,8);
        q.construction.spanCount = rndi(1,5);
        q.construction.wallHeight = rnd(3,4);
        q.construction.roofInclination = rnd(30,45);
        q.construction.infiltration = rnd(0.75,3);
        q.construction.end1 = randomCoverMaterial();
        q.construction.end2 = randomCoverMaterial();
        q.construction.side1= randomCoverMaterial();
        q.construction.side2= randomCoverMaterial();
        q.construction.roof1= randomCoverMaterial();
        q.construction.roof2= randomCoverMaterial();
        q.construction.floor.emissivity = rnd(0.7,0.9);
        q.construction.floor.Uindoors = rnd(5,8);
        q.construction.floor.Usoil = rnd(8,10);
        q.construction.floor.heatCapacity = rnd(30000, 50000);

        int n;
        n = rndi(1,4); RANDOM_VECTOR(heatPipes, randomHeatPipe);
        n = 1;         RANDOM_VECTOR(vents, randomVent);
        n = rndi(1,4); RANDOM_VECTOR(growthLights, randomGrowthLight);
        n = rndi(3,9); RANDOM_VECTOR(screens, randomScreen);

        q.co2Dispenser.injectionRate = rndv(0,10);
        q.dehumidifiers.size = 0;

        Box *calendar = MegaFactory::create<Box>("Calendar", "calendar", nullptr);
        calendar->port("latitude")->equals(q.greenhouse.latitude);
        calendar->port("longitude")->equals(q.greenhouse.longitude);
        calendar->port("timeZone")->equals(q.timeStamp.timeZone);
        calendar->port("initialDateTime")->equals(dateTime);
        calendar->initialize();
        calendar->reset();
        calendar->update();
        double angot = calendar->port("angot")->value<double>(),
               sinb = calendar->port("sinb")->value<double>();
        delete calendar;

        q.outdoors.temperature = rndv(-5,30);
        q.outdoors.irradiation = Variable{angot*sinb, rndo()};
        q.outdoors.rh = rndv(50,90);
        q.outdoors.co2 = rndv(360,410);
        q.outdoors.windspeed = rndv(0,15);
        q.outdoors.windDirection = rndv(0, 360);

        q.indoors.temperature = rndv(15,35);
        q.indoors.lightIntensity = Variable{q.outdoors.irradiation.value, rndo()};
        q.indoors.rh = rndv(50,90);
        q.indoors.co2 = rndv(450, 2000);

    }
    catch (Exception &ex) {
        std::cout << qPrintable("Unexpected exception in function randomQuery:\n" + ex.what());
    }

    return q;
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

//Box* buildIndoors(const Query &q) {
//    BoxBuilder builder;
//    builder.
//        box().name("indoors").
//            box().name("given").
//                box("vg::AirFluxGiven").name("airFlux").
//                    box("vg::AirFluxInfiltration").name("infiltration").
//                        port("leakage").equals(q.construction.infiltration).
//                    endbox().
//                    box().name("crackVentilation").
//                        newPort("value").equals(1e-9).
//                        newPort("state").equals(0).
//                    endbox().
//                    box("vg::AirFluxGravitation").name("gravitation").
//                    endbox().
//                endbox().
//                box("vg::VapourFluxSum").name("vapourFlux").
//                    box("vg::VapourFluxTranspiration").name("transpiration").
//                    endbox().
//                    box("vg::VapourFluxCondensation").name("condensationCover").
//                        port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]").
//                        port("surfaceTemperature").imports("energyFlux/shelter[coverTemperature]").
//                    endbox().
//                    box("vg::VapourFluxCondensation").name("condensationScreens").
//                        port("surfaceAreaPerGroundArea").imports("construction/shelter[screensPerGroundArea]").
//                        port("surfaceTemperature").imports("energyFlux/shelter[screensTemperature]").
//                    endbox().
//                    box("vg::VapourFluxAir").name("airFluxOutdoors").
//                        port("airFlux").imports("given/airFlux[value]").
//                    endbox().
//                endbox().
//                box("vg::EnergyFluxSum").name("energyFlux").
//                    box().name("light").
//                        newPort("value").imports("indoors/light[total]").
//                    endbox().
//                    box("vg::EnergyFluxCondensation").name("condensationCover").
//                        port("vapourFlux").imports("../../vapourFlux/condensationCover[vapourFlux]").
//                    endbox().
//                    box("vg::EnergyFluxCondensation").name("condensationScreens").
//                        port("vapourFlux").imports("../../vapourFlux/condensationScreens[vapourFlux]").
//                    endbox().
//                    box("vg::EnergyFluxAir").name("airFlux").
//                        port("airFlux").imports("given/airFlux[value]").
//                    endbox().
//                    box("vg::EnergyFluxGrowthLights").name("growthLights").
//                    endbox().
//                    box("vg::EnergyFluxShelters").name("shelter").
//                    endbox().
//                    box("vg::EnergyFluxFloor").name("floor").
//                        port("Uindoors").equals(q.construction.floor.Uindoors).
//                        port("Usoil").equals(q.construction.floor.Usoil).
//                        port("heatCapacity").equals(q.construction.floor.heatCapacity).
//                        port("emissivity").equals(q.construction.floor.emissivity).
//                        box("vg::FloorRadiationAbsorbed").name("radiationAbsorbed").
//                        endbox().
//                    endbox().
//                    box("vg::EnergyFluxTranspiration").name("transpiration").
//                    endbox().
//                endbox().
//            endbox().
//            box(buildControlled()).
//            box().name("total").
//                box("Sum").name("airflux").
//                    port("inputs").equals("(given/airFlux[value] cooling/airFlux[value])").
//                endbox().
//                box("VapourFluxSum").name("vapourFlux").
//                    port("toAdd").equals("(given/vapourFlux cooling/vapourFlux)").
//                endbox().
//                box("Sum").name("energyFlux").
//                    port("inputs").equals("(given/energyFlux[value] energyFlux/heating/supply[value] cooling/energyFlux[value])").
//                endbox().
//            endbox().
//            box("vg::IndoorsLight").name("light").
//            endbox().
//            box("vg::IndoorsTemperature").name("temperature").
//                port("resetValue").equals(18).
//                port("energyFlux").imports("total/energyFlux[value]").
//            endbox().
//            box("vg::IndoorsHumidity").name("humidity").
//            endbox().
//            box("vg::IndoorsCo2").name("co2").
//            endbox().
//            box("vg::IndoorsWindSpeed").name("windSpeed").
//            endbox().
//        endbox();
//    return builder.content(BoxBuilder::AmendNone);
//}

//Box* buildScreen(Screen screen) {
//    BoxBuilder builder;
//    builder.box().name("screen");
//    builder.endbox();
//    return builder.content();
//}

//Box* buildScreens(Screens screens) {
//    BoxBuilder builder;
//    builder.box().name("screens");
//    for (int i=0; i < screens.size; ++i) {
//        builder.box(buildScreen(&screens.array[i]));
//    }
//    builder.endbox();
//    return builder.content();
//}

void buildGrowthLight(Box *parent, const GrowthLight *g) {
    double age(0), lifeTime(0);
    if (g->age.origin!=NotAvailable && g->age.origin!=NotAvailable) {
        age = g->age.value;
        lifeTime = g->lifeTime.value;
    }
    double on(0);
    if (g->on.origin!=NotAvailable)
        on = g->on.value;

    BoxBuilder builder(parent);
    builder.
        box("vg::GrowthLight").name("growthLight").
            port("type").equals(toString(g->type)).
            port("intensity").equals(g->intensity).
            port("ballastCorrection").equals(g->ballastCorrection).
            port("age").equals(age).
            port("lifeTime").equals(lifeTime).
            port("on").equals(on).
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

//Box* buildPipes(HeatPipes pipes) {
//    BoxBuilder builder;
//    builder.box().name("pipes");
//    for (int i=0; i < pipes.size; ++i) {
//        builder.box(buildGrowthLight(&lights.array[i]));
//    }
//    builder.endbox();
//    return builder.content();
//}

void buildActuators(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
        box().name("actuators").
        endbox();
    Box *actuators = parent->findChild<Box*>("actuators");
    buildGrowthLights(actuators, q.growthLights);
//            box(buildPipes(q.heatPipes)).

}

struct Gauss {
    double
      xGaussUpperside,
      wGaussUpperside,
      xGaussLowerside,
      wGaussLowerside;
};

const Gauss    topGauss = Gauss{0.1127, 0.2778, 0.8873, 0.2778};
const Gauss middleGauss = Gauss{0.5000, 0.4444, 0.5000, 0.4444};
const Gauss  lowerGauss = Gauss{0.8873, 0.2778, 0.1127, 0.2778};

Box* buildLayer(QString name, Gauss g) {
    BoxBuilder builder;
    builder.
        box("LeafLayer").name(name).
            port("xGaussUpperside").equals(g.xGaussUpperside).
            port("wGaussUpperside").equals(g.wGaussUpperside).
            port("xGaussLowerside").equals(g.xGaussLowerside).
            port("wGaussLowerside").equals(g.wGaussLowerside).
            box("LeafWindSpeed").name("windSpeed").
                port("k").equals(0.6).
            endbox().
            box("StomatalResistanceRose").name("rs").
            endbox().
            box("BoundaryLayerResistanceStanghellini").name("rb").
            endbox().
            box("LeafRadiationAbsorbed").name("radiationAbsorbed").
            endbox().
            box("LeafTranspiration").name("transpiration").
            endbox().
            box("LeafTemperature").name("temperature").
            endbox().
            box("LeafPhotosynthesis").name("photosynthesis").
                box("LeafLightResponse").name("lightResponse").
                endbox().
            endbox().
        endbox();
    return builder.content(BoxBuilder::AmendNone);
}

Box* buildLayers() {
    BoxBuilder builder;
    builder.
        box().name("layers").
            box(buildLayer("top", topGauss)).
            box(buildLayer("middle", middleGauss)).
            box(buildLayer("bottom", lowerGauss)).
        endbox();
    return builder.content(BoxBuilder::AmendNone);
}

void buildSensor(Box *parent, const Query &q) {
    double indoorsAh = ahFromRh(q.indoors.temperature.value, q.indoors.rh.value),
           indoorsCo2  = (q.indoors.co2.origin!=NotAvailable) ? q.indoors.co2.value : 405.,
           outdoorsCo2 = (q.outdoors.co2.origin!=NotAvailable) ? q.outdoors.co2.value : 405.;

    BoxBuilder builder(parent);
    builder.
        box().name("sensor").
            newPort("indoorsTemperature").equals(q.indoors.temperature.value).
            newPort("indoorsRh").equals(q.indoors.rh.value).
            newPort("indoorsAh").equals(indoorsAh).
            newPort("indoorsCo2").equals(indoorsCo2).
            newPort("indoorsWindspeed").equals(0.1).
            newPort("outdoorsTemperature").equals(q.outdoors.temperature.value).
            newPort("outdoorsRh").equals(q.outdoors.rh.value).
            newPort("outdoorsCo2").equals(outdoorsCo2).
            newPort("outdoorsGlobalRadiation").equals(q.outdoors.irradiation.value).
            newPort("outdoorsPropParRadiation").equals(0.47).
            newPort("outdoorsWindSpeed").equals(q.outdoors.windspeed.value).
            newPort("soilTemperature").equals(10.).
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
            port("timeStep").equals(1).
            port("timeUnit").equals("s").
        endbox();
}

void buildOutdoors(Box *parent) {
    BoxBuilder builder(parent);
    builder.
        box("vg::Outdoors").name("outdoors").
            port("propParRadiation").imports("sensor[outdoorsPropParRadiation]").
            port("co2").imports("sensor[outdoorsCo2]").
            port("soilTemperature").imports("sensor[soilTemperature]").
            port("temperature").imports("sensor[outdoorsTemperature]").
            port("rh").imports("sensor[outdoorsRh]").
            port("radiation").imports("sensor[outdoorsGlobalRadiation]").
            port("windSpeed").imports("sensor[outdoorsWindSpeed]").
            port("diffuseRadiation").imports("./diffuseIrradiation[value]").
            port("skyTemperature").imports("./skyTemperature[temperature]").
            box("DiffuseIrradiationRE").name("diffuseIrradiation").
            endbox().
            box("SkyTemperature").name("skyTemperature").
            endbox().
            box().name("records").
            endbox().
        endbox();
}

Box* buildScreen(const Screen *s) {
    BoxBuilder builder;
    QString orientation = (s->position==WholeRoof) ? "horizontal" : "cover";
    double effect = (s->effect.origin!=NotAvailable) ? s->effect.value : 0.;
    builder.
        box("vg::Screen").name("screen").
            port("orientation").equals(orientation).
            port("transmissivityLight").equals(s->material.transmissivityLight).
            port("emissivityInner").equals(s->material.emmisivityInner).
            port("emissivityOuter").equals(s->material.emmisivityOuter).
            port("U").equals(s->material.U).
            port("energyLossReduction").equals(s->material.energySaving).
            port("haze").equals(s->material.haze).
            port("specificHeatCapacity").equals(s->material.heatCapacity).
            port("transmissivityAir").equals(s->material.transmissivityAir).
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

Box* buildVent(const Vents vents) {
    if (vents.size == 0)
        ThrowException("Missing vent");
    if (vents.size > 1)
        ThrowException("Only one vent allowed").value(vents.size);
    Vent v = *vents.array;
    double opening = (v.opening.origin!=NotAvailable) ? v.opening.value : 0.;

    BoxBuilder builder;
    builder.
        box("vg::Vent").name("vent").
            port("length").equals(v.length).
            port("width").equals(v.height).
            port("number").equals(v.numberOfVents).
            port("transmissivity").equals(v.porosity).
            port("state").equals(opening).
        endbox();
    return builder.content(BoxBuilder::AmendNone);
}

#define SHELTERFACE(X, Y) \
box("vg::ShelterFace").name(#X). \
    box("vg::Cover").name("cover"). \
        port("directTransmissionFile").equals(":/igclient/input/direct_transmission_single.txt"). \
        port("U4").equals(q.construction.X.U). \
        port("emissivity").equals(q.construction.X.emissivity). \
        port("absorptivity").equals(q.construction.X.absorptivity). \
        port("transmissivity").equals(q.construction.X.transmissivity). \
        port("haze").equals(q.construction.X.haze). \
        port("specificHeatCapacity").equals(q.construction.X.heatCapacity). \
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
                SHELTERFACE(roof1, Roof1).box(buildVent(q.vents)).endbox().
                SHELTERFACE(roof2, Roof2).endbox().
                SHELTERFACE(side1, Side1).endbox().
                SHELTERFACE(side2, Side2).endbox().
                SHELTERFACE(end1, End1).endbox().
                SHELTERFACE(end2, End2).endbox().
            endbox().
        endbox();
}

void buildIndoors(Box *parent) {
    BoxBuilder builder(parent);
    builder.
        box().name("indoors").
            box("vg::IndoorsLight").name("light").
            endbox().
            box().name("temperature").
                newPort("value").imports("sensor[indoorsTemperature]").
            endbox().
            box().name("humidity").
                newPort("rh").imports("sensor[indoorsRh]").
                newPort("ah").imports("sensor[indoorsAh]").
            endbox().
            box().name("windSpeed").
                newPort("value").imports("sensor[indoorsWindspeed]").
            endbox().
            box().name("co2").
                newPort("value").imports("sensor[indoorsCo2]").
            endbox().
        endbox().
        box("Given").name("given").
            box("AirFluxGiven").name("airFlux").
                box().name("crackVentilation").
                endbox().
            endbox().
        endbox();
}

void buildControllers(Box *parent) {
    BoxBuilder builder(parent);
    builder.
        box().name("controllers").
            box().name("chalk").
                newPort("value").equals(0.).
            endbox().
        endbox();
}

void buildCrop(Box *parent, const Query &q) {
    double lai = (q.culture.lai.origin!=NotAvailable) ? q.culture.lai.value : 1.;
    BoxBuilder builder(parent);
    builder.
        box("vg::Crop").name("crop").
            box("vg::CropLai").name("lai").
                port("laiStartPerPlant").equals(lai).
            endbox().
//            box(buildLayers()).
//            box("vg::CropRadiation").name("radiation").
//            endbox().
//            box("vg::Average").name("temperature").
//                port("inputs").imports("../layers/*/temperature[value]").
//            endbox().
//            box("vg::Sum").name("lightAbsorbed").
//                port("inputs").imports("../layers/*/radiationAbsorbed[lightAbsorbed]").
//            endbox().
//            box("vg::Sum").name("heatingAbsorbed").
//                port("inputs").imports("../layers/*/radiationAbsorbed[heatingAbsorbed]").
//            endbox().
//            box("vg::Sum").name("growthLightLwAbsorbed").
//                port("inputs").imports("layers/*/radiationAbsorbed[growthLightLwAbsorbed]").
//            endbox().
//            box("vg::Sum").name("radiationAbsorbed").
//                port("inputs").imports(" layers/*/radiationAbsorbed[lightAbsorbed]").
//            endbox().
//            box("vg::Average").name("conductance").
//                port("inputs").imports("layers/*/transpiration[conductance]").
//            endbox().
//            box("vg::Average").name("vapourFlux").
//                port("inputs").imports("layers/*/transpiration[vapourFlux]").
//            endbox().
//            box("vg::Average").name("gain").
//                port("inputs").imports("layers/*/transpiration[gain]").
//            endbox().
//            box("vg::CropGrowth").name("growth").
//                box("Sum").name("Pg"). // assimilation rate
//                    port("inputs").imports("layers/*/photosynthesis[Pg]").
//                endbox().
//            endbox().
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
    Box *sim;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(10).
            endbox();
        sim = builder.content();
        buildSensor(sim, q);
        buildCalendar(sim, q);
        buildOutdoors(sim);
        buildConstruction(sim, q);
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

double wave(double avg, double amplitude, double hour) {
    return avg + sin(2*3.1412*hour/48)*amplitude;
}

Response compute(const Query &q) {
    init();
    Response r;
    bool excepted = false;

    Box *root;
    try {
        root = build(q);
        root->run();
    }
    catch (Exception &ex) {
        _errorString = ex.what().toStdString();
        excepted = true;
    }
    if (excepted || root->port("hasError")->value<bool>()) {
        if (!excepted)
            _errorString = root->port("errorMsg")->value<QString>().toStdString();
        r.hasError = true;
        r.error = _errorString.c_str();
    }

    environment().latestLoadArg("igclient.box");
    Command::submit(QStringList() << "write", nullptr);

//    QString s;
//    QTextStream str(&s);
//    root->toText(str);
//    std::cout << qPrintable(s);

    // Extract response from model state
    try {
        r.timeStamp = q.timeStamp;
        r.photosynthesis = root->findOne<Box>("crop/growth")->port("netGrowthRate")->value<double>();
        r.growthLight = root->findOne<Box>("actuators/growthLights")->port("currentPower")->value<double>();
    }
    catch (Exception &ex) {
        std::cout << ex.what().toStdString() << "\n";
    }

    return r;
}

const char * queryToString(const Query &q) {
    Box *sim = build(q);

    QString s;
    QTextStream text(&s);
    sim->toText(text);

    delete sim;
    _queryString = text.string()->toStdString();
    return _queryString.c_str();
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
