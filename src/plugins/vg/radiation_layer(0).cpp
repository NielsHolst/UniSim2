/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/test_num.h>
#include "radiation_layer.h"

using namespace base;
using namespace TestNum;

namespace vg {

RadiationLayer::RadiationLayer(QString name, QObject *parent)
    : Box(name, parent)
{
    help("holds the radiative parameters of cover or screen");
    Class("RadiationLayer");
    Input(swReflectivityOut).equals(0.1).unit("[0;1]").help("Short-wave reflectivity outwards");
    Input(swReflectivityIn).equals(0.1).unit("[0;1]").help("Short-wave reflectivity inwards");
    Input(swTransmissivity).equals(0.9).unit("[0;1]").help("Short-wave transmissivity");
    Input(lwReflectivityOut).equals(0.1).unit("[0;1]").help("Long-wave reflectivity outwards");
    Input(lwReflectivityIn).equals(0.1).unit("[0;1]").help("Long-wave reflectivity inwards");
    Input(lwTransmissivity).equals(0.0).unit("[0;1]").help("Long-wave transmissivity");
    Output(swAbsorptanceOut).unit("[0;1]").help("Short-wave absorptance outwards");
    Output(swAbsorptanceIn).unit("[0;1]").help("Short-wave absorptance inwards");
    Output(lwEmissivityOut).unit("[0;1]").help("Long-wave emissivity outwards");
    Output(lwEmissivityIn).unit("[0;1]").help("Long-wave emissivity inwards");
}

void RadiationLayer::reset() {
    update();
}

#define CHK(x) checkRange(x, #x)

void RadiationLayer::update() {
    swAbsorptanceOut = 1. - swReflectivityOut - swTransmissivity;
    swAbsorptanceIn  = 1. - swReflectivityIn  - swTransmissivity;
    lwEmissivityOut  = 1. - lwReflectivityOut - lwTransmissivity;
    lwEmissivityIn   = 1. - lwReflectivityIn - lwTransmissivity;
    snapTo(swAbsorptanceOut, 0.); snapTo(swAbsorptanceOut, 1.);
    snapTo(swAbsorptanceIn , 0.); snapTo(swAbsorptanceIn , 1.);
    snapTo(lwEmissivityOut , 0.); snapTo(lwEmissivityOut , 1.);
    snapTo(lwEmissivityIn  , 0.); snapTo(lwEmissivityIn  , 1.);
    CHK(swReflectivityOut);
    CHK(swReflectivityIn);
    CHK(swTransmissivity);
    CHK(lwReflectivityOut);
    CHK(lwReflectivityIn);
    CHK(lwTransmissivity);
    CHK(swAbsorptanceOut);
    CHK(swAbsorptanceIn);
    CHK(lwEmissivityOut);
    CHK(lwEmissivityIn);
}

void RadiationLayer::checkRange(double x, QString name) const {
    if (x<0. || x>1.)
        ThrowException("Radiative parameter must be inside [0;1]").value(name).value(x);
}

double RadiationLayer::swTransmissivityTotal(QVector<const RadiationLayer*> layers) {
    double t(1), r(0) ;
    for (const RadiationLayer *layer : layers) {
        t *= layer->port("swTransmissivity")->value<double>();
        t *= 1./(1. - r*layer->port("swReflectivityOut")->value<double>());
        r = layer->port("swReflectivityIn")->value<double>();
    }
    return t;
}

double RadiationLayer::lwTransmissivityTotal(QVector<const RadiationLayer*> layers) {
    double t(1), r(0) ;
    for (const RadiationLayer *layer : layers) {
        t *= layer->port("lwTransmissivity")->value<double>();
        t *= 1./(1. - r*layer->port("lwReflectivityOut")->value<double>());
        r = layer->port("lwReflectivityIn")->value<double>();
    }
    return t;
}

QVector<double> RadiationLayer::lwEmissivityTotal(QVector<const RadiationLayer*> layers) {
     QVector<double> em(layers.size()-1);
     int i(0);
     for (double &e : em) {
         e = 1./(1./layers.at(i)->port("lwEmissivityIn")->value<double>() +
                 1./layers.at(i+1)->port("lwEmissivityOut")->value<double>() - 1.
                 );
     }
     return QVector<double>();
}


} //namespace

