#include <iostream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument >
#include <base/convert.h>
#include <base/exception.h>
#include "query_reader_json.h"

QueryReaderJson::QueryReaderJson(QObject *parent) : QObject(parent)
{
}

ig::Query QueryReaderJson::parse(QString filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        ThrowException("Cannot open JSON file").value(filePath);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if (doc.isNull())
        ThrowException("Cannot parse JSON file").
                value1(filePath).value2(error.errorString());
    if (doc.isArray()) {
        QJsonValue rootValue = doc.array().first();
        if (rootValue.isObject()) {
            QJsonObject rootObject = rootValue.toObject();
            std::cout << qPrintable(rootObject.keys().join("\n")) << "\n";
            parseQuery(rootObject);
        }
    }
    return _query;
}

QJsonValue QueryReaderJson::findValue(QJsonObject object, QString name) const {
    QJsonObject::const_iterator it = object.find(name);
    if (it==object.end())
        ThrowException("Cannot find JSON value").value(name);
    return it.value();
}

QJsonObject QueryReaderJson::findObject(QJsonObject object, QString name) const {
    QJsonValue value = findValue(object, name);
    if (!value.isObject())
        ThrowException("Found a JSON value but expected a JSON object").value(name);
    return value.toObject();
}

QJsonArray QueryReaderJson::findArray(QJsonObject object, QString name) const {
    QJsonValue value = findValue(object, name);
    if (!value.isArray())
        ThrowException("Found a JSON value but expected a JSON object").value(name);
    return value.toArray();
}

int QueryReaderJson::findInt(QJsonObject object, QString name) const {
    return findValue(object, name).toInt();
}

double QueryReaderJson::findDouble(QJsonObject object, QString name) const {
    return findValue(object, name).toDouble();
}

ig::Variable QueryReaderJson::findVariable(QJsonObject object, QString name) const {
    ig::Variable var;
    QJsonObject varObject = findObject(object, name);
    var.value = findDouble(varObject, "Value");
    var.origin = static_cast<ig::Origin>(findInt(varObject, "Origin"));
    return var;
}

ig::Variable QueryReaderJson::findVariableFromValue(QJsonObject object, QString name) const {
    ig::Variable var;
    QString s = findValue(object, name).toString();
    var.origin = (s=="null") ? ig::NotAvailable : ig::UserDefined;
    if (var.origin == ig::UserDefined)
        var.value = s.toDouble();
    return var;
}

#define PARSE_OBJECT(x) parse##x(findObject(object, #x))
#define PARSE_ARRAY(x)  parse##x( findArray(object, #x))

void QueryReaderJson::parseQuery(QJsonObject object) {
    parseTimeStamp(object);
    PARSE_OBJECT(Culture);
    PARSE_OBJECT(Construction);
    PARSE_ARRAY(HeatPipes);
    PARSE_ARRAY(Vents);
    PARSE_ARRAY(GrowthLights);
    PARSE_OBJECT(Co2Dispenser);
    PARSE_ARRAY(Screens);
    PARSE_ARRAY(Dehumidifiers);
    PARSE_OBJECT(OutdoorClimate);
    PARSE_OBJECT(IndoorClimate);
}

void QueryReaderJson::parseTimeStamp(QJsonObject object) {
    QJsonValue
        offset = findValue(object, "BaseUtcOffset"),
        ymdhms = findValue(object, "TimeStamp");
    // Example: "0001-01-01T00:00:00"
    QString s = ymdhms.toString();
    QStringList ss = s.split(QRegExp("[T\\-\\:]"));
    QList<int> numbers = base::convert<int, QList>(ss);
    if (numbers.size() != 6)
        ThrowException("Expected 6 integers in time stamp").value1(s).value2(numbers.size());
    int year = 2000+numbers.at(0)%4;
    QDate date(year, numbers.at(1), numbers.at(2));
    double hour = numbers.at(3) + numbers.at(4)/60. + numbers.at(5)/60./60.;
    // Fill in
    _query.timeStamp.dayOfYear = date.dayOfYear();
    _query.timeStamp.timeOfDay = hour;
    _query.timeStamp.timeZone = offset.toDouble();
}

void QueryReaderJson::parseCulture(QJsonObject) {
    // Not used
}

void QueryReaderJson::parseConstruction(QJsonObject object) {
    _query.construction.length = findDouble(object, "Length");
    _query.construction.spanWidth = findDouble(object, "SpanWidth");
    _query.construction.spanCount = findInt(object, "SpanCount");
    _query.construction.wallHeight = findDouble(object, "Length");
    _query.construction.roofInclination = findDouble(object, "RoofInclination");
    _query.construction.internalShading = findDouble(object, "InternalShading");
    _query.construction.infiltration = findDouble(object, "Infiltration");
    _query.construction.end1 = findCoverMaterial(object, "End1");
    _query.construction.end2 = findCoverMaterial(object, "End2");
    _query.construction.side1 = findCoverMaterial(object, "Side1");
    _query.construction.side2 = findCoverMaterial(object, "Side2");
    _query.construction.roof1 = findCoverMaterial(object, "Roof1");
    _query.construction.roof2 = findCoverMaterial(object, "Roof2");
    _query.construction.floor = findFloorMaterial(object, "Floor");
}

ig::CoverMaterial QueryReaderJson::findCoverMaterial(QJsonObject object, QString name) const {
    QJsonObject cover = findObject(object, name);
    ig::CoverMaterial cm;
    cm.emissivity = findDouble(cover, "Emissivity");
    cm.absorptivity = findDouble(cover, "Absorptivity");
    cm.transmissivity = findDouble(cover, "Transmissivity");
    cm.haze = findDouble(cover, "Haze");
    cm.U = findDouble(cover, "UValue");
    cm.heatCapacity = findDouble(cover, "HeatCapacity");
    return cm;
}

ig::FloorMaterial QueryReaderJson::findFloorMaterial(QJsonObject object, QString name) const {
    QJsonObject floor = findObject(object, name);
    ig::FloorMaterial fm;
    fm.emissivity  = findDouble(floor, "Emissivity");
    fm.Uindoors  = findDouble(floor, "UIndoors");
    fm.Usoil  = findDouble(floor, "USoil");
    fm.heatCapacity  = findDouble(floor, "HeatCapacity");
    return fm;
}

void QueryReaderJson::parseHeatPipes(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseHeatPipe(it->toObject());
    }
    _query.heatPipes.array = _heatPipes.data();
    _query.heatPipes.size = _heatPipes.size();
}

void QueryReaderJson::parseHeatPipe(QJsonObject object) {
    ig::HeatPipe hp;
    hp.material = static_cast<ig::HeatPipeMaterial>(findInt(object, "Material"));
    hp.flowRate  = findVariable(object, "FlowRate");
    hp.temperatureInflow = findVariableFromValue(object, "FlowTemperature");
    hp.temperatureOutflow = ig::Variable{0., ig::NotAvailable};
    hp.innerDiameter = findDouble(object, "InnerDiameter");
    hp.outerDiameter = findDouble(object, "OuterDiameter");
    hp.length = findDouble(object, "Length");
    _heatPipes.push_back(hp);
}

void QueryReaderJson::parseVents(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseVent(it->toObject());
    }
    _query.vents.array = _vents.data();
    _query.vents.size = _vents.size();
}

void QueryReaderJson::parseVent(QJsonObject object) {
    ig::Vent vent;
    vent.length = findDouble(object, "Length");
    vent.height = findDouble(object, "Height");
    vent.numberOfVents = findDouble(object, "NumberOfVents");
    vent.maxOpening = findDouble(object, "MaxOpening");
    vent.porosity = findDouble(object, "Porosity");
    vent.opening = findVariable(object, "Opening");
    _vents.push_back(vent);
}

void QueryReaderJson::parseGrowthLights(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseGrowthLight(it->toObject());
    }
    _query.growthLights.array = _growthLights.data();
    _query.growthLights.size = _growthLights.size();
}

void QueryReaderJson::parseGrowthLight(QJsonObject object) {
    ig::GrowthLight gl;
    gl.type = static_cast<ig::GrowthLightType>(findInt(object, "Type"));
    gl.intensity = findDouble(object, "Intensity");
    gl.ballastCorrection = findDouble(object, "BallactCorrection");
    gl.age = findVariable(object, "Age");
    gl.lifeTime = findVariable(object, "LifeTime");
    gl.on = findVariable(object, "On");
    _growthLights.push_back(gl);
}

void QueryReaderJson::parseScreens(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseScreen(it->toObject());
    }
    _query.screens.array = _screens.data();
    _query.screens.size = _screens.size();
}

void QueryReaderJson::parseScreen(QJsonObject object) {
    ig::Screen screen;
    screen.material = parseScreenMaterial(findObject(object, "Material"));
//    ScreenMaterial material;
//    ScreenLayer layer;
//    ScreenPosition position;
//    Variable effect;                // 0..1

    _screens.push_back(screen);
}

ig::ScreenMaterial QueryReaderJson::parseScreenMaterial(QJsonObject object) {
    ig::ScreenMaterial mat;
    mat.transmissivityLight = findDouble(object, "TransmissivityLight");
    mat.emmisivityInner = findDouble(object, "EmmisivityInner");
    mat.emmisivityOuter  = findDouble(object, "EmmisivityOuter");
    mat.haze  = findDouble(object, "Haze");
    mat.energySaving  = findDouble(object, "EnergySaving");
    mat.transmissivityAir  = findDouble(object, "TransmissivityAir");
    mat.U  = findDouble(object, "U");
    mat.heatCapacity = findDouble(object, "HeatCapacity");
    return mat;
}

void QueryReaderJson::parseDehumidifiers(QJsonArray objects) {
}

void QueryReaderJson::parseCo2Dispenser(QJsonObject) {
    _query.co2Dispenser.injectionRate = ig::Variable{0., ig::NotAvailable};
}

void QueryReaderJson::parseOutdoorClimate(QJsonObject object) {
}

void QueryReaderJson::parseIndoorClimate(QJsonObject object) {
}


