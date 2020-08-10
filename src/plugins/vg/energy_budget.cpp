/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "heat_transfer_volume.h"
#include "heat_transfer_layer_base.h"
#include "energy_budget.h"

#include <base/dialog.h>

using namespace base;

//#define LOG(x) dialog().information(x);
#define LOG(x)

namespace vg {

const double tolerance = 1e-3;

PUBLISH(EnergyBudget)

static QString layerLabels =
    "    a"
    "    r"
    "    t"
    "   a_"
    "   r_"
    "   t_"
    "   La"
    "   Lr"
    "   Lt"
    "  La_"
    "  Lr_"
    "  Lt_"
    "    T"
    "     E"
    "    E_"
    "    LE"
    "   LE_"
    "     A"
    "    A_"
    "    LA"
    "   LA_"
    "     U"
    "    U_";

static QString airLabels =
    "    V"
    "    T"
    "    Q";

QString EnergyBudget::toString(const HeatTransferLayerBase *layer) const {
    return
        QString::number(layer->  swAbsorptivityTop, 'f', 2).rightJustified(5) +
        QString::number(layer->  swReflectivityTop, 'f', 2).rightJustified(5) +
        QString::number(layer->swTransmissivityTop, 'f', 2).rightJustified(5) +
        QString::number(layer->  swAbsorptivityBottom, 'f', 2).rightJustified(5) +
        QString::number(layer->  swReflectivityBottom, 'f', 2).rightJustified(5) +
        QString::number(layer->swTransmissivityBottom, 'f', 2).rightJustified(5) +
        QString::number(layer->  lwAbsorptivityTop, 'f', 2).rightJustified(5) +
        QString::number(layer->  lwReflectivityTop, 'f', 2).rightJustified(5) +
        QString::number(layer->lwTransmissivityTop, 'f', 2).rightJustified(5) +
        QString::number(layer->  lwAbsorptivityBottom, 'f', 2).rightJustified(5) +
        QString::number(layer->  lwReflectivityBottom, 'f', 2).rightJustified(5) +
        QString::number(layer->lwTransmissivityBottom, 'f', 2).rightJustified(5) +
        QString::number(layer->temperature, 'f', 1).rightJustified(5) +
        QString::number(layer->swFluxDown,  'f', 1).rightJustified(6) +
        QString::number(layer->swFluxUp  ,  'f', 1).rightJustified(6) +
        QString::number(layer->lwFluxDown,  'f', 0).rightJustified(6) +
        QString::number(layer->lwFluxUp  ,  'f', 0).rightJustified(6) +
        QString::number(layer->swAbsorbedFromAbove, 'f', 1).rightJustified(6) +
        QString::number(layer->swAbsorbedFromBelow, 'f', 1).rightJustified(6) +
        QString::number(layer->lwAbsorbedFromAbove, 'f', 0).rightJustified(6) +
        QString::number(layer->lwAbsorbedFromBelow, 'f', 0).rightJustified(6) +
        QString::number(layer->Utop, 'f', 1).rightJustified(6) +
        QString::number(layer->Ubottom, 'f', 1).rightJustified(6);
}

QString EnergyBudget::toString(const HeatTransferVolume *airSpace) const {
    return
        QString::number(airSpace->volume, 'f', 1).rightJustified(5) +
        QString::number(airSpace->temperature, 'f', 1).rightJustified(5) +
        QString::number(airSpace->convectiveInflux, 'f', 1).rightJustified(5);
}

QString EnergyBudget::toString() const {
    QString s = QString().fill(' ', 12) + layerLabels;
    for (HeatTransferLayerBase *layer : stack)
        s += "\n" + layer->objectName().leftJustified(12) + toString(layer);
    s += "\n" + QString().fill(' ', 12) + airLabels;
    s += "\n" + airSpaceScreened->objectName().leftJustified(12) + toString(airSpaceScreened);
    s += "\n" + airSpaceRoom    ->objectName().leftJustified(12) + toString(airSpaceRoom);
    return s;
}


EnergyBudget::EnergyBudget(QString name, QObject *parent)
    : Box(name, parent)
{
    help("distributes radiation among layers");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(screenedVolume).imports("construction/geometry[screenedVolume]");
    Input(roomVolume).imports("construction/geometry[roomVolume]");
    Input(cropCoverage).imports("crop[coverage]",CA);
    Input(withCrop).equals(true).unit("bool").help("Layers over crop?");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(soilTemperature).imports("outdoors[soilTemperature]");
}

void EnergyBudget::amend() {
    bool
        hasScreen1 = !findMany<Box>("construction/shelter/*/screens/layer1").isEmpty(),
        hasScreen2 = !findMany<Box>("construction/shelter/*/screens/layer2").isEmpty(),
        hasScreen3 = !findMany<Box>("construction/shelter/*/screens/layer3").isEmpty();
    BoxBuilder builder(this);
    builder.
    box("HeatTransferSky").name("sky").
    endbox().
    box("HeatTransferCover").name("cover").
    endbox();
    if (hasScreen1)
        builder.
        box("HeatTransferScreen1").name("screen1").
        endbox();
    if (hasScreen2)
        builder.
        box("HeatTransferScreen2").name("screen2").
        endbox();
    if (hasScreen3)
        builder.
        box("HeatTransferScreen3").name("screen3").
        endbox();
    builder.
    box("HeatTransferPipe").name("pipe").
    endbox().
    box("HeatTransferCrop").name("crop").
    endbox().
    box("HeatTransferFloor").name("floor").
    endbox().
    box().name("airSpaces").
        box("HeatTransferVolume").name("screened").
            port("volume").imports("construction/geometry[screenedVolume]", CA).
            port("outdoorsInfluxVolume").imports("ventilation/net[screened]", CA).
            port("neighbourInfluxVolume").imports("construction/geometry[screenedVolumeChange]", CA).
            port("neighbourTemperature").imports("../indoors[temperature]", CA).
        endbox().
        box("HeatTransferVolume").name("indoors").
            port("volume").imports("construction/geometry[roomVolume]", CA).
            port("outdoorsInfluxVolume").imports("ventilation/net[room]", CA).
            port("neighbourInfluxVolume").imports("construction/geometry[roomVolumeChange]", CA).
            port("neighbourTemperature").imports("../screened[temperature]", CA).
        endbox().
    endbox();
}

void EnergyBudget::reset() {
    HeatTransferLayerBase *screen1 = findMaybeOne<HeatTransferLayerBase>("./screen1"),
                   *screen2 = findMaybeOne<HeatTransferLayerBase>("./screen2"),
                   *screen3 = findMaybeOne<HeatTransferLayerBase>("./screen3");
    stack.clear();
    stack << findOne<HeatTransferLayerBase>("./sky")
          << findOne<HeatTransferLayerBase>("./cover");

    screens.clear();
    if (screen1) {
        stack << screen1;
        screens << screen1;
    }
    if (screen2) {
        stack << screen2;
        screens << screen2;
    }
    if (screen3) {
        stack << screen3;
        screens << screen3;
    }

    stack << findOne<HeatTransferLayerBase>("actuators/growthLights")
          << findOne<HeatTransferLayerBase>("./pipe")
          << findOne<HeatTransferLayerBase>("./crop")
          << findOne<HeatTransferLayerBase>("./floor");

    airSpaceScreened = findOne<HeatTransferVolume>("./airSpaces/screened");
    airSpaceRoom     = findOne<HeatTransferVolume>("./airSpaces/indoors");

    update();
}

void EnergyBudget::update() {
    LOG("EnergyBudget::update()\n" + toString());
//    area = withCrop ? groundArea*cropCoverage : groundArea*(1.-cropCoverage);
    distributeSwRadiation();
    distributeLwRadiation();
    distributeParRadiation();
    int n = stack.size();
    for (int i=0; i<n; ++i) {
        stack[i]->absorbed = stack.at(i)->swAbsorbed + stack.at(i)->lwAbsorbed;
    }
    distributeHeatByConvectionAndConduction();
}

void EnergyBudget::distributeParRadiation() {
    int n = stack.size();
    Vec a(n), r(n), t(n), a_(n), r_(n), t_(n),
        I(n), I_(n), A(n), A_(n);
    for (int i=0; i<n; ++i) {
        a[i]  = stack.at(i)->swAbsorptivityTop;
        r[i]  = stack.at(i)->swReflectivityTop;
        t[i]  = stack.at(i)->swTransmissivityTop;
        a_[i] = stack.at(i)->swAbsorptivityBottom;
        r_[i] = stack.at(i)->swReflectivityBottom;
        t_[i] = stack.at(i)->swTransmissivityBottom;
        I[i]  = stack.at(i)->parFluxDown;
        I_[i] = stack.at(i)->parFluxUp;
        LOG("I1:   " + QString::number(I.at(i)));
    }
    distributeRadiation(a, r, t, a_, r_, t_, I, I_, A, A_);
    for (int i=0; i<n; ++i) {
        stack[i]->parFluxDown          = I .at(i);
        stack[i]->parFluxUp            = I_.at(i);
        stack[i]->parAbsorbedFromAbove = A .at(i);
        stack[i]->parAbsorbedFromBelow = A_.at(i);
        stack[i]->parAbsorbed          = A .at(i) + A_.at(i);
    }
}

void EnergyBudget::distributeSwRadiation() {
    int n = stack.size();
    Vec a(n), r(n), t(n), a_(n), r_(n), t_(n),
        I(n), I_(n), A(n), A_(n);
    for (int i=0; i<n; ++i) {
        a[i]  = stack.at(i)->swAbsorptivityTop;
        r[i]  = stack.at(i)->swReflectivityTop;
        t[i]  = stack.at(i)->swTransmissivityTop;
        a_[i] = stack.at(i)->swAbsorptivityBottom;
        r_[i] = stack.at(i)->swReflectivityBottom;
        t_[i] = stack.at(i)->swTransmissivityBottom;
        I[i]  = stack.at(i)->swFluxDown;
        I_[i] = stack.at(i)->swFluxUp;
    }
    distributeRadiation(a, r, t, a_, r_, t_, I, I_, A, A_);
    for (int i=0; i<n; ++i) {
        stack[i]->swFluxDown          = I .at(i);
        stack[i]->swFluxUp            = I_.at(i);
        stack[i]->swAbsorbedFromAbove = A .at(i);
        stack[i]->swAbsorbedFromBelow = A_.at(i);
        stack[i]->swAbsorbed          = A .at(i) + A_.at(i);
    }
}

void EnergyBudget::distributeLwRadiation() {
    int n = stack.size();
    Vec a(n), r(n), t(n), a_(n), r_(n), t_(n),
        I(n), I_(n), A(n), A_(n);
    for (int i=0; i<n; ++i) {
        a[i]  = stack.at(i)->lwAbsorptivityTop;
        r[i]  = stack.at(i)->lwReflectivityTop;
        t[i]  = stack.at(i)->lwTransmissivityTop;
        a_[i] = stack.at(i)->lwAbsorptivityBottom;
        r_[i] = stack.at(i)->lwReflectivityBottom;
        t_[i] = stack.at(i)->lwTransmissivityBottom;
        I[i]  = stack.at(i)->lwFluxDown;
        I_[i] = stack.at(i)->lwFluxUp;
    }
    distributeRadiation(a, r, t, a_, r_, t_, I, I_, A, A_);
    for (int i=0; i<n; ++i) {
        stack[i]->lwFluxDown          = I .at(i);
        stack[i]->lwFluxUp            = I_.at(i);
        stack[i]->lwAbsorbedFromAbove = A .at(i);
        stack[i]->lwAbsorbedFromBelow = A_.at(i);
        stack[i]->lwAbsorbed          = A .at(i) + A_.at(i);
    }
}

QString EnergyBudget::toString(
        Vec a, Vec r, Vec t,
        Vec a_, Vec r_, Vec t_,
        Vec I, Vec I_, Vec F, Vec F_, Vec A, Vec A_
        ) {
    QString s =
        " i"
        "       a"
        "       r"
        "       t"
        "      a_"
        "      r_"
        "      t_"
        "       I"
        "      I_"
        "       F"
        "      F_"
        "       A"
        "      A_\n";
    for (int i=0; i<a.size(); ++i) {
        s +=
        QString::number(i+1)             .rightJustified(2) +
        QString::number(a.at(i),  'f', 3).rightJustified(8) +
        QString::number(r.at(i),  'f', 3).rightJustified(8) +
        QString::number(t.at(i),  'f', 3).rightJustified(8) +
        QString::number(a_.at(i), 'f', 3).rightJustified(8) +
        QString::number(r_.at(i), 'f', 3).rightJustified(8) +
        QString::number(t_.at(i), 'f', 3).rightJustified(8) +
        QString::number(I.at(i),  'f', 3).rightJustified(8) +
        QString::number(I_.at(i), 'f', 3).rightJustified(8) +
        QString::number(F.at(i),  'f', 3).rightJustified(8) +
        QString::number(F_.at(i), 'f', 3).rightJustified(8) +
        QString::number(A.at(i),  'f', 3).rightJustified(8) +
        QString::number(A_.at(i), 'f', 3).rightJustified(8) + "\n";
    }
    return s;
}

void EnergyBudget::distributeRadiation(
        Vec a, Vec r, Vec t,
        Vec a_, Vec r_, Vec t_,
        Vec I, Vec I_, Vec &A, Vec &A_
        )
{
    Vec F = I, F_ = I_;
    A.fill(0.);
    A_.fill(0.);
    int i=0;
    do {
        LOG("RadiationLayers::distributeRadiation() " + QString::number(i) + "a:\n" + toString(a, r, t, a_, r_, t_, I, I_, F, F_, A, A_));
        distributeRadiationDown(a, r, t, a_, r_, t_, F, F_, A, A_);
        LOG("RadiationLayers::distributeRadiation() " + QString::number(i) + "b:\n" + toString(a, r, t, a_, r_, t_, I, I_, F, F_, A, A_));
        distributeRadiationUp  (a, r, t, a_, r_, t_, F, F_, A, A_);
        LOG("RadiationLayers::distributeRadiation() " + QString::number(i) + "c:\n" + toString(a, r, t, a_, r_, t_, I, I_, F, F_, A, A_));
    } while (vector_op::sum(F) + vector_op::sum(F_) > tolerance && ++i<20);
    if (i==20)
        ThrowException("DistributeRadiation did not converge").
                value(vector_op::sum(I)).
                value2(vector_op::sum(I_));
}

/* F contains the downward, F_ the upward radiation flux.
 * F will be distributed among F_ (by reflection), A and A_ (absorbed fluxes downwards and upwards),
 * adding to their current values.
 * After distribution F will be zero.
 */
void EnergyBudget::distributeRadiationDown(
        Vec a, Vec r, Vec t,
        Vec a_, Vec r_, Vec t_,
        Vec &F, Vec &F_, Vec &A, Vec &A_
     )
{
    int n = a.size();
    for (int i=0; i<n-1; ++i) {
        double
            k = 1. - r[i+1]*r_[i],
            th  = t[i+1]/k,
            ah  = a[i+1]/k,
            th_ = t_[i] /k * r[i+1],
            ah_ = a_[i] /k * r[i+1];
        F [i+1] += th *F[i];
        A [i+1] += ah *F[i];
        F_[i]   += th_*F[i];
        A_[i]   += ah_*F[i];
    }
    F.fill(0.);
}

/* F contains the downward, F_ the upward radiation flux.
 * F_ will be distributed among F (by reflection), A and A_ (absorbed fluxes downwards and upwards),
 * adding to their current values.
 * After distribution F_ will be zero.
 */
void EnergyBudget::distributeRadiationUp(
        Vec a, Vec r, Vec t,
        Vec a_, Vec r_, Vec t_,
        Vec &F, Vec &F_, Vec &A, Vec &A_
     )
{
    int n = a.size();
    for (int i=n-1; i>0; --i) {
        double
            k = 1. - r_[i-1]*r[i],
            th_ = t_[i-1]/k,
            ah_ = a_[i-1]/k,
            th  = t[i] /k * r_[i-1],
            ah  = a[i] /k * r_[i-1];
        F_[i-1] += th_*F_[i];
        A_[i-1] += ah_*F_[i];
        F [i]   += th *F_[i];
        A [i]   += ah *F_[i];
    }
    F_.fill(0.);
}

inline double powdiff(double Tdiff) {
    return pow(fabs(Tdiff), 0.33)*Tdiff;
}

void EnergyBudget::distributeHeatByConvectionAndConduction() {
    // Accumulators for total heat loss from screened and room volumes
    double totalHeatFromScreened = 0.,
           totalHeatFromRoom     = 0.;

    // Convection at cover to outdoors and to screened volume
    double propScreened = screenedVolume/(screenedVolume+roomVolume);
    HeatTransferLayerBase *cover = stack[1];
    // W/m2 surface = W/m2/K * K
    double heatFromOuterToCover    = cover->Utop   *(outdoorsTemperature - cover->temperature),
           heatFromScreenedToCover = cover->Ubottom*powdiff(airSpaceScreened->temperature - cover->temperature)*propScreened,
           heatFromRoomToCover     = cover->Ubottom*powdiff(airSpaceRoom->temperature     - cover->temperature)*(1.-propScreened),
           totalHeatToCover        = heatFromOuterToCover + heatFromScreenedToCover;
    // W = W/m2 * m2
    totalHeatFromScreened          += heatFromScreenedToCover*cover->area;
    totalHeatFromRoom              += heatFromRoomToCover;
    LOG(QString("A totalHeatFromScreened %1 W").arg(totalHeatFromScreened));
    QString s("Test %1 %2 %3 %4");
    LOG(s.arg(cover->Ubottom).arg(airSpaceScreened->temperature).arg(cover->temperature).arg(cover->area));

    // Convection at both sides of screens (excluding the innermost) to screened volume
    int numScreens = screens.size();
    QVector<double> totalHeatToScreen(numScreens);
    for (int i=0; i<numScreens-1; ++i) {
        // W/m2 surface = W/m2/K * K
        double heat = (screens.at(i)->Utop + screens.at(i)->Ubottom)*powdiff(airSpaceScreened->temperature - screens.at(i)->temperature);
        totalHeatToScreen[i]   = heat;
        // W = W/m2 * m2
        totalHeatFromScreened += heat*screens.at(i)->area;
        LOG(QString("B%1 totalHeatFromScreened %2").arg(i).arg(totalHeatFromScreened));
    }

    // Convection at innermost screen to screened volume and room volume
    int i = numScreens-1;
    // W/m2 surface = W/m2/K * K
    double heatFromScreenedToScreen  = screens.at(i)->Utop   *powdiff(airSpaceScreened->temperature - screens.at(i)->temperature),
           heatFromRoomToScreen      = screens.at(i)->Ubottom*powdiff(airSpaceRoom->temperature - screens.at(i)->temperature);
    totalHeatToScreen[i]   = heatFromScreenedToScreen + heatFromRoomToScreen;
    // W = W/m2 * m2
    totalHeatFromScreened += heatFromScreenedToScreen*screens.at(i)->area;
    totalHeatFromRoom     += heatFromRoomToScreen    *screens.at(i)->area;
    LOG(QString("C totalHeatFromScreened %1").arg(totalHeatFromScreened));
    LOG(QString("A totalHeatFromRoom %1").arg(totalHeatFromRoom));

    // Convection between floor, and room volume and soil
    HeatTransferLayerBase *floor = stack.last();
    // W/m2 ground = W/m2/K * K
    double heatFromRoomToFloor = floor->Utop*powdiff(airSpaceRoom->temperature - floor->temperature),
           heatFromSoilToFloor = floor->Ubottom*(soilTemperature - floor->temperature);
    // W = W/m2 * m2
    totalHeatFromRoom         += heatFromRoomToFloor*floor->area;
    LOG(QString("B totalHeatFromRoom %1").arg(totalHeatFromRoom));

    // Transfer energy to layers (W/m2 surface)
    cover->convectiveInflux = totalHeatToCover;
    LOG(QString("Z totalHeatToCover %1").arg(totalHeatToCover));
    for (int i=0; i<numScreens; ++i) {
        screens[i]->convectiveInflux = totalHeatToScreen.at(i);
        LOG(QString("Z%1 totalHeatToScreen %2").arg(i).arg(totalHeatToScreen.at(i)));
    }
    // W/m2 ground
    floor->convectiveInflux = heatFromRoomToFloor;
    floor->conductiveInflux = heatFromSoilToFloor;
    LOG(QString("Z heatFromRoomToFloor %1").arg(heatFromRoomToFloor));
    LOG(QString("Z heatFromSoilToFloor %1").arg(heatFromSoilToFloor));
    // Transfer energy to air volumes (W)
    airSpaceScreened->convectiveInflux = -totalHeatFromScreened;
    airSpaceRoom->convectiveInflux     = -totalHeatFromRoom;
}

//}

} //namespace

