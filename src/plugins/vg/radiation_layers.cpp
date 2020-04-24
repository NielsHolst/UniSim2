/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "air_space_base.h"
#include "radiation_layer.h"
#include "radiation_layers.h"

using namespace base;

namespace vg {

PUBLISH(RadiationLayers)

RadiationLayers::RadiationLayers(QString name, QObject *parent)
    : Box(name, parent)
{
    help("distributes radiation among layers");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(cropCoverage).imports("crop[coverage]",CA);
    Input(withCrop).equals(true).unit("bool").help("Layers over crop?");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(soilTemperature).imports("outdoors[soilTemperature]");
    Output(newIndoorsTemperature).unit("oC").help("Update indoors temperature");
}

void RadiationLayers::amend() {
    BoxBuilder builder(this);
    builder.
    box("RadiationLayerSky").name("sky").
    endbox().
    box("RadiationLayerCover").name("cover").
    endbox().
    box("RadiationLayerScreen1").name("screen1").
    endbox().
    box("RadiationLayerScreen2").name("screen2").
    endbox().
    box("RadiationLayerScreen3").name("screen3").
    endbox().
    box("RadiationLayerPipe").name("pipe").
    endbox().
    box("RadiationLayerCrop").name("crop").
    endbox().
    box("RadiationLayerFloor").name("floor").
    endbox() ;
}

void RadiationLayers::initialize() {
    RadiationLayer *screen1 = findOne<RadiationLayer>("./screen1"),
                   *screen2 = findOne<RadiationLayer>("./screen2"),
                   *screen3 = findOne<RadiationLayer>("./screen3");
    stack.clear();
    stack << findOne<RadiationLayer>("./sky")
          << findOne<RadiationLayer>("./cover");

    if (screen1->port("numFaces",CA)->value<int>() > 0)
        stack << screen1;
    if (screen2->port("numFaces",CA)->value<int>() > 0)
        stack << screen2;
    if (screen3->port("numFaces",CA)->value<int>() > 0)
        stack << screen3;

    stack << findOne<RadiationLayer>("growthLights")
          << findOne<RadiationLayer>("./pipe")
          << findOne<RadiationLayer>("./crop")
          << findOne<RadiationLayer>("./floor");

    airSpaces << findMany<AirSpaceBase>("shelter/*/screens/*/airSpace")
              << findOne<AirSpaceBase>("construction/airSpaceNet");
}


void RadiationLayers::reset() {
    indoorsTemperature = 20.;
    update();
}

void RadiationLayers::update() {
    area = withCrop ? groundArea*cropCoverage : groundArea*(1.-cropCoverage);
    newIndoorsTemperature = indoorsTemperature;
    distributeSwRadiation(stack);
    distributeLwRadiation(stack);
    int n = stack.size();
    for (int i=0; i<n; ++i) {
        stack[i]->absorbed = stack.at(i)->swAbsorbed + stack.at(i)->lwAbsorbed;
    }
//        distributeHeatByConvection(stack);
//        distributeHeatByConduction(stack);
}

void RadiationLayers::distributeParRadiation(Stack &stack) {
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
        A[i]  = stack.at(i)->parAbsorbedFromAbove;
        A_[i] = stack.at(i)->parAbsorbedFromBelow;
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

void RadiationLayers::distributeSwRadiation(Stack &stack) {
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
        A[i]  = stack.at(i)->swAbsorbedFromAbove;
        A_[i] = stack.at(i)->swAbsorbedFromBelow;
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

void RadiationLayers::distributeLwRadiation(Stack &stack) {
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
        A[i]  = stack.at(i)->lwAbsorbedFromAbove;
        A_[i] = stack.at(i)->lwAbsorbedFromBelow;
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

void RadiationLayers::distributeRadiation(
        Vec a, Vec r, Vec t,
        Vec a_, Vec r_, Vec t_,
        Vec &I, Vec &I_, Vec &A, Vec &A_
        )
{
    int i=0;
    do {
        distributeRadiationDown(a, r, t, a_, r_, t_, I, I_, A, A_);
        distributeRadiationUp  (a, r, t, a_, r_, t_, I, I_, A, A_);
    } while (vector_op::sum(I) + vector_op::sum(I_) > 1e-6 && ++i<20);
    if (i==20)
        ThrowException("DistributeRadiation did not converge").
                value(vector_op::sum(I)).
                value2(vector_op::sum(I_));
}

void RadiationLayers::distributeRadiationDown(
        Vec a, Vec r, Vec t,
        Vec a_, Vec r_, Vec t_,
        Vec &I, Vec &I_, Vec &A, Vec &A_
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
        I [i+1] += th *I[i];
        A [i+1] += ah *I[i];
        I_[i]   += th_*I[i];
        A_[i]   += ah_*I[i];
    }
    I.fill(0.);
}

void RadiationLayers::distributeRadiationUp(
        Vec a, Vec r, Vec t,
        Vec a_, Vec r_, Vec t_,
        Vec &I, Vec &I_, Vec &A, Vec &A_
        )
{
    int n = a.size();
    for (int i=n; i>0; --i) {
        double
            k = 1. - r_[i-1]*r[i],
            th_ = t_[i-1]/k,
            ah_ = a_[i-1]/k,
            th  = t[i] /k * r_[i-1],
            ah  = a[i] /k * r_[i-1];
        I_[i-1] += th_*I_[i];
        A_[i-1] += ah_*I_[i];
        I [i]   += th *I_[i];
        A [i]   += ah *I_[i];
    }
    I_.fill(0.);
}

inline double Uadj(double U, double Tdiff) {
    return U*cbrt(fabs(Tdiff));
}

struct HeatExchange {
    double heatFromOuterToScreen, heatFromInnerToScreen;
};

HeatExchange calcHeatExchange(double outerTemperature, double screenTemperature, double innerTemperature,
                              double Utop, double Ubottom) {
//    double Utop    = Uadj(screen->Utop,     outerTemperature - screenTemperature),
//           Ubottom = Uadj(screen->Ubottom,  innerTemperature - screenTemperature);
    HeatExchange he;
    he.heatFromOuterToScreen = Utop   *(outerTemperature - screenTemperature);
    he.heatFromInnerToScreen = Ubottom*(innerTemperature - screenTemperature);
    return he;
}

void RadiationLayers::distributeHeatByConvection(Stack &stack) {
//    for (int i=0; i<7; ++i) {
//        const Stack &stack(stacks.at(i));
//        AirSpaceStack &airSpaceStack(airSpaceStacks[i]);

//        RadiationLayer *cover = stack[1];
//        AirSpaceBase *innerAirSpace = airSpaceStack[0];
//        double outerTemperature = outdoorsTemperature,
//               coverTemperature = cover->temperature,
//               innerTemperature = innerAirSpace->temperature,
//               heatFromOuterToCover = cover->Utop   *(outerTemperature - coverTemperature),
//               heatFromInnerToCover = cover->Ubottom*(innerTemperature - coverTemperature);
//        cover->temperature    += heatFromOuterToCover + heatFromInnerToCover;
//        innerAirSpace->temperature -= heatFromInnerToCover;

//        int n = airSpaceStack.size();
//        for (int j=1; j<n; ++j) {
//            AirSpaceBase *outerAirSpace = airSpaceStack[j-1];
//            RadiationLayer *screen = stack[j+1];
//            AirSpaceBase *innerAirSpace = airSpaceStack[j];
//            HeatExchange he = calcHeatExchange(
//                   outerAirSpace->temperature,
//                   screen->temperature,
//                   innerAirSpace->temperature,
//                   screen->Utop,
//                   screen->Ubottom);
//            outerAirSpace->temperature -= he.heatFromOuterToScreen;
//            screen->temperature        += he.heatFromOuterToScreen + he.heatFromInnerToScreen;
//            innerAirSpace->temperature -= he.heatFromOuterToScreen;
//        }

//        AirSpaceBase *outerAirSpace = airSpaceStack[n-1];
//        RadiationLayer *screen = stack[n+1];
//        HeatExchange he = calcHeatExchange(
//               outerAirSpace->temperature,
//               screen->temperature,
//               indoorsTemperature,
//               screen->Utop,
//               screen->Ubottom);
//        outerAirSpace->temperature -= he.heatFromOuterToScreen;
//        screen->temperature        += he.heatFromOuterToScreen + he.heatFromInnerToScreen;
//        newIndoorsTemperature      -= he.heatFromOuterToScreen;
//        // Ignore convection at plant and floor
//    }
}

void RadiationLayers::distributeHeatByConduction(Stack &stack) {

}

//void RadiationLayers::heatTransferOuterSurface(double fromTemperature, double toTemperature) const {
//    convection.heat.transfer.from.outside = function(Tout, Tlayer, wind.speed=4) {
//    double h = 7.2 + 3.8*wind.speed
//      h*(Tout - Tlayer)
//    }

//}

//void RadiationLayers::heatTransferInnerSurface(double fromTemperature, double toTemperature) const {

//}

} //namespace

