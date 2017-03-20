#define IGLIB_DLL dllexport

#include <QString>
#include <base/convert.h>
#include <base/exception.h>
#include "iglib.h"

using namespace base;
using std::string;

namespace ig {

namespace {
    QStringList split(QString name, QString s, int n) {
        QStringList items = s.split(" ", QString::SkipEmptyParts);
        if (items.size() != n+1) {
            QString msg("Expected %1 space-separated items in %2");
            ThrowException(msg.arg(n).arg(name)).value(s).value2(items.size());
        }
        if (items.at(0) != name) {
            QString msg("Expected '%1'");
            ThrowException(msg.arg(name)).value(items.at(0));
        }
        items.removeFirst();
        return items;
    }
}

QString convert(Origin origin) {
    switch (origin) {
    case UserDefined: return "UserDefined";
    case Measured: return "Measured";
    case NotAvailable: return "NotAvailable";
    }
    return QString();
}


Origin convertOrigin(QString s) {
    if (s=="UserDefined") return UserDefined;
    if (s=="Measured") return Measured;
    if (s!="NotAvailable")
        ThrowException("Unknown origin").value(s);
    return NotAvailable;
}

QString convert(CultureType type) {
    switch (type) {
    case PotRose : return "PotRose";
    case PotChrysanthemum : return "PotChrysanthemum";
    case Cucumber : return "Cucumber";
    case Tomato : return "Tomato";
    case Lettuce : return "Lettuce";
    case SweetPepper : return "SweetPepper";
    }
    return QString();
}


CultureType convertCultureType(QString s) {
    if (s=="PotRose") return PotRose;
    if (s=="PotChrysanthemum") return PotChrysanthemum;
    if (s=="Cucumber") return Cucumber;
    if (s=="Tomato") return Tomato;
    if (s=="Lettuce") return Lettuce;
    if (s!="SweetPepper")
        ThrowException("Unknown CultureType").value(s);
    return SweetPepper;
}

QString convert(HeatPipeMaterial type) {
    switch (type) {
    case Iron : return "Iron";
    case Plastic : return "Plastic";
    case Aluminium : return "Aluminium";
    }
    return QString();
}

HeatPipeMaterial convertHeatPipeMaterial(QString s) {
    if (s=="Iron") return Iron;
    if (s=="Plastic") return Plastic;
    if (s!="Aluminium")
        ThrowException("Unknown HeatPipeMaterial").value(s);
    return Aluminium;
}


QString toString(Variable v)  {
    return base::convert<QString>(v.value) + " " + convert(v.origin);
}

Variable fromStringVariable(QString s) {
    Variable v;
    QStringList items = split("Variable", s, 2);
    v.value = base::convert<double>(items.at(0));
    v.origin = convertOrigin(items.at(1));
    return v;
}

QString toString(TimeStamp t) {
    return "TimeStamp "
            + base::convert<QString>(t.dayOfYear) + " "
            + base::convert<QString>(t.timeOfDay) + " "
            + base::convert<QString>(t.timeZone);
}

TimeStamp fromStringTimeStamp(QString s) {
    TimeStamp t;
    QStringList items = split("TimeStamp", s, 3);
    t.dayOfYear = base::convert<int>(items.at(0));
    t.timeOfDay = base::convert<double>(items.at(1));
    t.timeZone = base::convert<double>(items.at(2));
    return t;
}

typedef QVector<double *> Doubles;

QString convert(Doubles v) {
    QStringList l;
    for (double *x : v)
        l << QString::number(*x);
    return l.join(" ");
}

void convert(Doubles v, QStringList l) {
    Q_ASSERT (v.size() != l.size());
    int i = 0;
    for (QString s : l.toVector())
        *(v[i++]) = base::convert<double>(s);
}

//
// Greenhouse
//
Doubles doubles(Greenhouse g) {
    return Doubles()
            << &g.latitude
            << &g.longitude
            << &g.direction
            << &g.length
            << &g.width;
}

Greenhouse fromStringGreenhouse(QString s) {
    Greenhouse g;
    convert(doubles(g), split("Greenhouse", s, 5));
    return g;
}

QString toString(Greenhouse g) {
   return "Greenhouse " + convert(doubles(g));
}

//
// Culture
//

QString toString(Culture c) {
    return "Culture "
            + convert(c.type) + " "
            + toString(c.lai);
}

Culture fromStringCulture(QString s) {
    Culture c;
    QStringList items = split("TimeStamp", s, 1);
    c.type = convertCultureType(items.at(0));
    c.lai = fromStringVariable(items.at(1));
    return c;
}

//
// CoverMaterial
//
Doubles doubles(CoverMaterial c) {
    return Doubles()
            << &c.emissivity
            << &c.absorptivity
            << &c.transmissivity
            << &c.haze
            << &c.U
            << &c.heatCapacity;
}

CoverMaterial fromStringCoverMaterial(QString s) {
    CoverMaterial c;
    convert(doubles(c), split("CoverMaterial", s, 6));
    return c;
}

QString toString(CoverMaterial c) {
   return "CoverMaterial " + convert(doubles(c));
}

//
// FloorMaterial
//
Doubles doubles(FloorMaterial f) {
    return Doubles()
            << &f.emissivity
            << &f.Uindoors
            << &f.Usoil
            << &f.heatCapacity;
}

FloorMaterial fromStringFloorMaterial(QString s) {
    FloorMaterial f;
    convert(doubles(f), split("FloorMaterial", s, 4));
    return f;
}

QString toString(FloorMaterial f) {
   return "FloorMaterial " + convert(doubles(f));
}

//
// Construction
//
Doubles doubles(Construction c) {
    return Doubles()
            << &c.internalShading
            << &c.length
            << &c.width
            << &c.wallHeight
            << &c.roofInclination
            << &c.spanWidth
            << &c.infiltration;
}

Construction fromStringConstruction(QString s) {
    Construction c;
    convert(doubles(c), split("Construction", s, 7));
    c.spanCount = base::convert<int>(s.at(7));
    return c;
}

QString toString(Construction c) {
   return "Construction " + convert(doubles(c)) + " " + QString::number(c.spanCount);
}

//
// HeatPipe
//

HeatPipe fromStringHeatPipe(QString s) {
    HeatPipe p;
    QStringList l = split("HeatPipe", s, 13);
    int i = 0;
    p.material = convertHeatPipeMaterial(s.at(i++));
    p.flowRate = fromStringVariable(s.at(i++) + " " + s.at(i++));
    p.temperatureInflow = fromStringVariable(s.at(i++) + " " + s.at(i++));
    p.temperatureOutflow = fromStringVariable(s.at(i++) + " " + s.at(i++));
    p.innerDiameter = fromStringVariable(s.at(i++) + " " + s.at(i++));
    p.outerDiameter = fromStringVariable(s.at(i++) + " " + s.at(i++));
    p.length = fromStringVariable(s.at(i++) + " " + s.at(i++));
    return p;
}

QString toString(HeatPipe p) {
   return "HeatPipe "
           + convert(p.material) + " "
           + toString(p.flowRate) + " "
           + toString(p.temperatureInflow) + " "
           + toString(p.temperatureOutflow) + " "
           + toString(p.innerDiameter) + " "
           + toString(p.outerDiameter) + " "
           + toString(p.length);
}

//
// HeatPipes
//
HeatPipes fromStringHeatPipes(QString head, QString tail) {
    HeatPipes p;
    QStringList l = split("HeatPipes", head, 1);
    p.size = base::convert<int>(l.at(0));
    p.heatPipe = new HeatPipe[p.size];
    for (int i = 0; i < p.size; ++i)
        p.heatPipe[i] = fromStringHeatPipe()
    return p;
}

QString toString(HeatPipes p) {
   QString s = "HeatPipes " + " " + QString::number(p.size) + "\n";
   for (int i = 0; i < p.size; ++i)
       s += toString(p.heatPipe[i]) + "\n";
   return s;
}

//
// Query
//
string toString(Query q) {
    QString s = toString(q.timeStamp)
            + "\n" + toString(q.greenhouse)
            + "\n" + toString(q.culture)
            + "\n" + toString(q.construction)
            + "\n" + toString(q.construction.end1)
            + "\n" + toString(q.construction.end2)
            + "\n" + toString(q.construction.side1)
            + "\n" + toString(q.construction.side2)
            + "\n" + toString(q.construction.roof1)
            + "\n" + toString(q.construction.roof2)
            + "\n" + toString(q.construction.floor)
            + "\n" + toString(q.heatPipes)
            ;
   return s.toStdString();
}


} // namespace
