#define IGLIB_DLL dllexport
#include <iostream>
#include <math.h>
#include <string>
#include <QCoreApplication>
#include <QDateTime>
#include <QString>
#include <QTextStream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/dialog_quiet.h>
#include <base/exception.h>
#include <base/object_pool.h>
#include <base/mega_factory.h>
#include "iglib.h"

using namespace base;

namespace ig {

// Flags
static bool _initialized = false;
static bool _released = false;
// Global objects
static QCoreApplication *_app = 0;
static DialogBase *_dialog = 0;
static Box *_rnd;
// Buffers
static std::string _queryString, _responseString, _errorString;
QVector<HeatPipe> _heatPipes;
QVector<GrowthLight> _growthLights;
QVector<Vent> _vents;
QVector<Screen> _screens;

void init() {
    static int argc = 1;
    static char ch = 0;
    static char *argv = &ch;
    if (_released)
        ThrowException("iglib has been released; library functions cannot be called");
    if (!_initialized) {
        _app = new QCoreApplication(argc, &argv);
        new ObjectPool(_app);
        _dialog = new DialogQuiet(_app);

        _rnd = MegaFactory::create<Box>("RandomUniform", "rnd", _app);
        _rnd->port("drawAtUpdate")->equals(true);
        _rnd->initialize();

        _initialized = true;
    }
}

double rnd(double floor, double ceiling) {
    _rnd->update();
    double value = _rnd->port("value")->value<double>();
    value = floor + value*(ceiling - floor);
    return (value*100)/100;
}

int rndi(int floor, int ceiling) {
    return (int) rnd(floor,ceiling);
}

Origin rndo() {
    return (Origin) rndi(0,3);
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
    h.innerDiameter = Variable{d - rnd(0.5,1), UserDefined};
    h.outerDiameter = Variable{d, UserDefined};
    h.length = rndv(0.5, 1.5);
    return h;
}

Vent randomVent() {
    Vent v;
    v.length = rnd(2,5);
    v.height = rnd(0.5,1);
    v.numberOfVents = rndi(5,10);
    v.maxOpening = rndi(70,120);
    v.porosity = rndi(0.9,1);
    v.opening = rndv(0,1);
    return v;
}

GrowthLight randomGrowthLight() {
    GrowthLight g;
    g.type = (GrowthLightType) rndi(0,2) ;
    g.intensity = rndi(20,50);
    g.ballastCorrection = 1 + 0.05*rndi(0,3);
    double v =rnd(1000,10000);
    Origin o = rndo();
    g.age = Variable{v, o};
    g.lifeTime = Variable{v*rnd(5,10), o};
    g.on = Variable{(double) rndi(0,2), Measured};
    return g;
}

Screen randomScreen() {
    Screen s;
    s.material.transmissivityLight = rnd(0.3,0.6);
    s.material.emmisivityInner = rnd(0.3,0.7);
    s.material.emmisivityOuter = rnd(0.05, 0.6);
    s.material.haze = rnd(0,0.1);
    s.material.energySaving = rnd(0.3,0.6);
    s.material.transmissivityAir = rnd(0.7,0.9);
    s.material.U = rnd(4,8);
    s.material.heatCapacity = rnd(300,700);
    s.layer = (ScreenLayer) rndi(0,3);
    s.position = (ScreenPosition) rndi(0,8);
    s.effect = rndv(0,1);
    return s;
}

#define RANDOM_VECTOR(X,Y,N) \
_##X.resize(N); \
for (int i=0; i<N; ++i)  _##X[i] = Y(); \
q.X.size = n; \
q.X.array = _##X.data()


Query randomQuery() {
    init();
    Query q;
    try {
        QDateTime dateTime = randomDateTime();
        double latitude = rnd(-65,65),
               longitude = rnd(-180,180);
        q.timeStamp.dayOfYear = dateTime.date().dayOfYear();
        q.timeStamp.timeOfDay = dateTime.time().hour() + dateTime.time().minute()/60.;
        q.timeStamp.timeZone = (int) (longitude/15. + 0.5);
        q.greenhouse.latitude = latitude;
        q.greenhouse.longitude = longitude;
        q.greenhouse.direction = rndi(0,360);
        q.culture.type = (CultureType) rndi(0,5);
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
        n = rndi(1,4);
        RANDOM_VECTOR(heatPipes, randomHeatPipe, n);

        n = rndi(1,3);
        RANDOM_VECTOR(vents, randomVent, n);

        n = rndi(1,4);
        RANDOM_VECTOR(growthLights, randomGrowthLight, n);

        n = rndi(3,9);
        RANDOM_VECTOR(screens, randomScreen, n);

        q.co2Dispenser.injectionRate = rndv(0,10);
        q.dehumidifiers.size = 0;

        Box *calendar = MegaFactory::create<Box>("Calendar", "calendar", 0);
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

#define SHELTER(X, Y) \
box("vg::Shelter").name(#X). \
    box("vg::Cover").name("cover"). \
        port("directTransmissionFile").equals("input/direct_transmission_single.txt"). \
        port("U4").equals(q.construction.X.U). \
        port("emissivity").equals(q.construction.X.emissivity). \
        port("absorptivity").equals(q.construction.X.absorptivity). \
        port("transmissivity").equals(q.construction.X.transmissivity). \
        port("haze").equals(q.construction.X.haze). \
        port("antiReflection").equals(false). \
        port("specificHeatCapacity").equals(q.construction.X.heatCapacity). \
    endbox(). \
    box(buildScreens(q.screens, Y)). \
endbox()

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
//    {Hpsl, Led}
Box* buildScreen(const Screen *s) {
    BoxBuilder builder;
    builder.
        box("vg::Screen").name("screen").
            port("position").equals(toString(s->position)).
            port("layer").equals(toString(s->layer)).
            port("transmissivityLight").equals(s->material.transmissivityLight).
            port("emissivityInner").equals(s->material.emmisivityInner).
            port("emissivityOuter").equals(s->material.emmisivityOuter).
            port("U").equals(s->material.U).
            port("energyLossReduction").equals(s->material.energySaving).
            port("haze").equals(s->material.haze).
            port("specificHeatCapacity").equals(s->material.heatCapacity).
            port("transmissivityAir").equals(s->material.transmissivityAir).
        endbox();
    return builder.content();
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
    return builder.content();
}

Box* buildVent(const Vent *v) {
    BoxBuilder builder;
    builder.
        box("vg::Vent").name("vent").
            port("length").equals(v->length).
            port("height").equals(v->height).
            port("number").equals(v->numberOfVents).
            port("porosity").equals(v->porosity).
            port("maxOpening").equals(v->maxOpening).
        endbox();
    return builder.content();
}

Box* buildVents(Vents vents) {
    BoxBuilder builder;
    builder.box("vg::Vents").name("vents");
    for (int i=0; i < vents.size; ++i) {
        builder.box(buildVent(&vents.array[i]));
    }
    builder.endbox();
    return builder.content();
}

Box* buildGrowthLight(const GrowthLight *g) {
    double age(0), lifeTime(0);
    if (g->age.origin!=NotAvailable && g->age.origin!=NotAvailable) {
        age = g->age.value;
        lifeTime = g->lifeTime.value;
    }
    double on(0);
    if (g->on.origin!=NotAvailable)
        on = g->on.value;

    BoxBuilder builder;
    builder.
        box("vg::GrowthLight").name("growthLight").
            port("type").equals(toString(g->type)).
            port("intensity").equals(g->intensity).
            port("ballastCorrection").equals(g->ballastCorrection).
            port("age").equals(age).
            port("lifeTime").equals(lifeTime).
            port("on").equals(on).
        endbox();
    return builder.content();
}

Box* buildGrowthLights(GrowthLights lights) {
    BoxBuilder builder;
    builder.box("vg::GrowthLights").name("growthLights");
    for (int i=0; i < lights.size; ++i) {
        builder.box(buildGrowthLight(&lights.array[i]));
    }
    builder.endbox();
    return builder.content();
}

Box* buildOutdoors(const Query &q) {
    Variable diffuseIrradiation =
        Variable{q.outdoors.irradiation.value/2,
                 q.outdoors.irradiation.origin};
    BoxBuilder builder;
    builder.
        box("vg::Outdoors").name("outdors").
            port("propParRadiation").equals(0.47).
            port("co2").equals(VAR(q.outdoors.co2)).
            port("soilTemperature").equals(10).
            port("temperature").equals(VAR(q.outdoors.temperature)).
            port("rh").equals(VAR(q.outdoors.rh)).
            port("radiation").equals(VAR(q.outdoors.irradiation)).
            port("diffuseRadiation").equals(VAR(diffuseIrradiation)).
            port("windSpeed").equals(VAR(q.outdoors.windspeed)).
            port("skyTemperature").equals(VAR(q.outdoors.temperature)).
        endbox();
    return builder.content();
}

Box* buildConstruction(const Query &q) {
    BoxBuilder builder;
    builder.
        box().name("construction").
            box("vg::ConstructionGeometry").name("geometry").
                port("numSpans").equals(q.construction.spanCount).
                port("spanWidth").equals(q.construction.spanWidth).
                port("length").equals(q.construction.length).
                port("height").equals(q.construction.wallHeight).
                port("margin").equals(0.15).
                port("roofPitch").equals(q.construction.roofInclination).
                port("shade").equals(q.construction.internalShading).
            endbox().
            box("vg::Shelters").name("shelters").
                SHELTER(roof1, Roof1).
                SHELTER(roof2, Roof2).
                SHELTER(side1, Side1).
                SHELTER(side2, Side2).
                SHELTER(end1, End1).
                SHELTER(end2, End2).
            endbox().
            box(buildVents(q.vents)).
        endbox();
    return builder.content();
}

Box* buildIndoors(const Query &q) {
    BoxBuilder builder;
    builder.
        box().name("indoors").
            box().name("given").
                box("vg::AirFluxGiven").name("airFlux").
                    box("vg::AirFluxInfiltration").name("infiltration").
                        port("leakage").equals(q.construction.infiltration).
                    endbox().
                    box().name("crackVentilation").
                        newPort("value").equals(0).
                        newPort("state").equals(0).
                    endbox().
                    box("vg::AirFluxGravitation").name("gravitation").
                    endbox().
                endbox().
                box("vg::VapourFluxSum").name("vapourFlux").
                    box("vg::VapourFluxTranspiration").name("transpiration").
                    endbox().
                    box("vg::VapourFluxCondensation").name("condensationCover").
                        port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]").
                        port("surfaceTemperature").imports("energyFlux/shelter[coverTemperature]").
                    endbox().
                    box("vg::VapourFluxCondensation").name("condensationScreens").
                        port("surfaceAreaPerGroundArea").imports("construction/shelters[screensPerGroundArea]").
                        port("surfaceTemperature").imports("energyFlux/shelter[screensTemperature]").
                    endbox().
                    box("vg::VapourFluxAir").name("airFluxOutdoors").
                        port("airFlux").imports("given/airFlux[value]").
                    endbox().
                endbox().
                box("vg::EnergyFluxSum").name("energyFlux").
                    box().name("light").
                        newPort("value").imports("indoors/light[total]").
                    endbox().
                    box("vg::EnergyFluxCondensation").name("condensationCover").
                        port("vapourFlux").imports("../../vapourFlux/condensationCover[vapourFlux]").
                    endbox().
                    box("vg::EnergyFluxCondensation").name("condensationScreens").
                        port("vapourFlux").imports("../../vapourFlux/condensationScreens[vapourFlux]").
                    endbox().
                    box("vg::EnergyFluxAir").name("airFlux").
                        port("airFlux").imports("given/airFlux[value]").
                    endbox().
                    box("vg::EnergyFluxGrowthLights").name("growthLights").
                    endbox().
                    box("vg::EnergyFluxShelters").name("shelter").
                    endbox().
                    box("vg::EnergyFluxFloor").name("floor").
                        port("Uindoors").equals(q.construction.floor.Uindoors).
                        port("Usoil").equals(q.construction.floor.Usoil).
                        port("heatCapacity").equals(q.construction.floor.heatCapacity).
                        port("emissivity").equals(q.construction.floor.emissivity).
                        box("vg::FloorRadiationAbsorbed").name("radiationAbsorbed").
                        endbox().
                        box("vg::EnergyFluxTranspiration").name("transpiration").
                        endbox().
                    endbox().
                endbox().
            endbox().
            box().name("controlled").
            endbox().
            box().name("total").
            endbox().
            box("vg::IndoorsLight").name("light").
            endbox().
            box("vg::IndoorsTemperature").name("temperature").
                port("resetValue").equals(18).
                port("energyFlux").imports("total/energyFlux[value]").
            endbox().
            box("vg::IndoorsHumidity").name("humidity").
            endbox().
            box("vg::IndoorsCo2").name("co2").
            endbox().
            box("vg::IndoorsWindSpeed").name("windSpeed").
            endbox().
        endbox();
    return builder.content();
}

Box* buildActuators(const Query &q) {
    BoxBuilder builder;
    builder.
        box().name("actuators").
            box(buildGrowthLights(q.growthLights)).
        endbox();
    return builder.content();
}

Box* build(const Query &q) {
    init();
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            port("steps").equals(1).
            box(buildOutdoors(q)).
            box(buildConstruction(q)).
            box(buildIndoors(q)).
            box(buildActuators(q)).
            box("OutputBuffer").
                port("ports").equals("(sim[step])").
                endbox().
        endbox();
    return builder.content();
}

double wave(double avg, double amplitude, double hour) {
    return avg + sin(2*3.1412*hour/48)*amplitude;
}

Response compute(const Query &q) {
    init();
    Response r;
    Box *sim(0);
    try {
        sim = build(q);
        sim->run();
        _errorString.clear();
        r.hasError = false;
    }
    catch (Exception &ex) {
        _errorString = ex.what().toStdString();
        r.hasError = true;
    }
    r.error = _errorString.c_str();

    double light(0);
    for (int i=0; i<q.growthLights.size; ++i) {
        const GrowthLight &g(q.growthLights.array[i]);
        light += g.on.value*g.intensity/g.ballastCorrection;
    }

    r.timeStamp = q.timeStamp;
    r.lightOutdoors = 4.57*q.outdoors.irradiation.value; // www.egc.com/useful_info_lighting.php
    r.lightArtificial = 4.59*light;
    r.lightPlantHeight = 0.78*r.lightOutdoors + r.lightArtificial;
    r.photosynthesisPct = 100*std::max(1-exp(-0.0065*(r.lightPlantHeight - 14.1)), 0.);
    r.photosynthesisRate = 8.3*1e-6*44.01*3600*r.photosynthesisPct/100;
    r.co2Indoors = wave(400, 6*r.photosynthesisPct, q.timeStamp.timeOfDay);

    delete sim;
    return r;
}

const char * queryToString(const Query &q) {
    Box *sim = build(q);

    QString s;
    QTextStream text(&s);
    sim->toText(text, Box::ToTextOptions::InputsOnly);

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
    RESP(photosynthesisRate);
    RESP(photosynthesisPct);
    RESP(lightOutdoors);
    RESP(lightArtificial);
    RESP(lightPlantHeight);
    RESP(co2Indoors);
    if (r.hasError)
        text << "ERROR:\n" << qPrintable(QString::fromStdString(r.error)) << "\n";
    else
        text << "No errors\n";
    _responseString = text.string()->toStdString();
    return _responseString.c_str();
}

void release() {
    _released = true;
    _dialog->deleteLater();
    _app->deleteLater();
    _dialog->deleteLater();
}

} // namespace
