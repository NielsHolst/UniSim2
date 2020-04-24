/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/test_num.h>
#include <base/vector_op.h>
#include "radiation_layer_shelter.h"

using namespace base;

namespace vg {

// See also the simpler definition of Input
#define InputShelter(X)  (*new base::Port(#X "Shelter", this)).data(& X##Shelter).access(base::PortAccess::Input) \
    .imports(path + "[" #X "]")

RadiationLayerShelter::RadiationLayerShelter(QString name, QObject *parent, QString path)
    : RadiationLayer(name, parent)
{
    InputShelter(swReflectivityTop);
    InputShelter(swReflectivityBottom);
    InputShelter(swTransmissivityTop);
    InputShelter(swTransmissivityBottom);
    InputShelter(swAbsorptivityTop);
    InputShelter(swAbsorptivityBottom);

    InputShelter(lwReflectivityTop);
    InputShelter(lwReflectivityBottom);
    InputShelter(lwTransmissivityTop);
    InputShelter(lwTransmissivityBottom);
    InputShelter(lwAbsorptivityTop);
    InputShelter(lwAbsorptivityBottom);

    InputShelter(Utop);
    InputShelter(Ubottom);
    InputShelter(area);

    Output(numFaces).help("Number of shelter faces contributing to this shelter layer").unit("0..6");
}

void RadiationLayerShelter::initialize() {
    numFaces = swReflectivityTopShelter.size();
    update();
}

void RadiationLayerShelter::reset() {
    update();
}

#define SUM_OF_PRODUCTS(x) x = vector_op::sumOfProducts(x##Shelter, areaShelter, this)

void RadiationLayerShelter::updateRadiativeProperties() {
    if (numFaces==0) return;
    SUM_OF_PRODUCTS(swReflectivityTop);
    SUM_OF_PRODUCTS(swReflectivityBottom);
    SUM_OF_PRODUCTS(swTransmissivityTop);
    SUM_OF_PRODUCTS(swTransmissivityBottom);
    SUM_OF_PRODUCTS(swAbsorptivityTop);
    SUM_OF_PRODUCTS(swAbsorptivityBottom);

    SUM_OF_PRODUCTS(lwReflectivityTop);
    SUM_OF_PRODUCTS(lwReflectivityBottom);
    SUM_OF_PRODUCTS(lwTransmissivityTop);
    SUM_OF_PRODUCTS(lwTransmissivityBottom);
    SUM_OF_PRODUCTS(lwAbsorptivityTop);
    SUM_OF_PRODUCTS(lwAbsorptivityBottom);

    double
        swTopSum    = swReflectivityTop    + swTransmissivityTop    + swAbsorptivityTop,
        swBottomSum = swReflectivityBottom + swTransmissivityBottom + swAbsorptivityBottom,
        lwTopSum    = lwReflectivityTop    + lwTransmissivityTop    + lwAbsorptivityTop,
        lwBottomSum = lwReflectivityBottom + lwTransmissivityBottom + lwAbsorptivityBottom;

    swReflectivityTop      /= swTopSum;
    swTransmissivityTop    /= swTopSum;
    swAbsorptivityTop      /= swTopSum;
    swReflectivityBottom   /= swBottomSum;
    swTransmissivityBottom /= swBottomSum;
    swAbsorptivityBottom   /= swBottomSum;

    lwReflectivityTop      /= lwTopSum;
    lwTransmissivityTop    /= lwTopSum;
    lwAbsorptivityTop      /= lwTopSum;
    lwReflectivityBottom   /= lwBottomSum;
    lwTransmissivityBottom /= lwBottomSum;
    lwAbsorptivityBottom   /= lwBottomSum;
}

void RadiationLayerShelter::updateConvectiveProperties() {
    if (numFaces==0) return;
    Utop    = vector_op::weightedAverage(UtopShelter, areaShelter, this);
    Ubottom = vector_op::weightedAverage(UbottomShelter, areaShelter, this);
}

} //namespace

