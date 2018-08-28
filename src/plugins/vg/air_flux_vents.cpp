/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/test_num.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "air_flux_vents.h"
#include "vent.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(AirFluxVents)

AirFluxVents::AirFluxVents(QString name, QObject *parent)
    : AirFluxBase(name, parent)
{
    help("computes air flux through a vent");
    Input(pathToVents).equals("shelter/*/*<Vent>").help("Path to Vent boxes");
    Input(ventOpening).imports("actuators/vents[value]");
    Input(roofPitch).imports("geometry[roofPitch]");
    Input(greenhouseLength).imports("geometry[length]");
    Input(indoorsVolumeTotal).imports("geometry[indoorsVolumeTotal]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(windSpeed).imports("outdoors[windSpeed]");
    Output(ventLength).noReset().help("Length scaled to two vents of greenhouse length (m)");
    Output(ventWidth).noReset().help("Average vent width weighted by length (m)");
    Output(ventTransmissivity).noReset().help("Average vent air transmissivity weighted by area [0;1]");
}

void AirFluxVents::initialize() {
    vents = findMany<Vent>(pathToVents);
    sumAreas = 0.;
    double
        sumWidth = 0,
        sumLength = 0,
        sumWeightedWidth = 0;
    for (Vent *vent : vents) {
        double
            number = vent->port("number")->value<int>(),
            length = vent->port("length")->value<double>(),
            width  = vent->port("width")->value<double>(),
            area = number*width*length;
        sumLength += number*length;
        sumWidth += width;
        sumWeightedWidth += length*width;
        areas << area;
        sumAreas += area;
    }
    int n = vents.size();
    ventLength = sumLength/n;
    ventWidth = sumWeightedWidth/sumLength;
}

void AirFluxVents::reset() {
    update();
}

inline double sind(double degrees) {
    return sin(degrees/180.*PI);
}

void AirFluxVents::update() {
    const double Al = 0.664; // discharge coefficient
    const double Cw = 0.24;  // wind pressure coefficient
    const double n  = 1.45;  // empirical constant

    updateTransmissivity();
    double angle = ventOpening*(90.-roofPitch),
           S = 2*ventLength*ventWidth*sind(angle/2.),
           H = (ventLength > 0.) ? S/ventLength : 0.,
           dT = std::max(indoorsTemperature-outdoorsTemperature, 0.),
           G = S/2*Al*sqrt(2*g*dT/(outdoorsTemperature+T0)*H/4 + Cw*pow(windSpeed,n)); // m3/s
    value = ventTransmissivity*G/indoorsVolumeTotal*3600.;
}

void AirFluxVents::updateTransmissivity() {
    double sumWeightedTransmissivity = 0;
    int i = 0;
    for (Vent *vent : vents) {
        double transmissivity = vent->port("transmissivity")->value<double>();
        sumWeightedTransmissivity += transmissivity*areas.at(i++);
    }
    ventTransmissivity = sumAreas>0. ? sumWeightedTransmissivity/sumAreas : 0.;
    TestNum::snapToZero(ventTransmissivity);
    TestNum::snapTo(ventTransmissivity, 1.);
    Q_ASSERT(ventTransmissivity<=1.);
}

} //namespace

