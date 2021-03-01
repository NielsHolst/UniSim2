/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/phys_math.h>
#include "canopy_photosynthesis.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(CanopyPhotosynthesis)

CanopyPhotosynthesis::CanopyPhotosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes canopy light capture and photosynthetic rate");
    Input(parAbsorbed).imports("energyBudget/crop[parAbsorbed]");
    Input(lai).imports("..[lai]",CA);
    Input(k).imports("energyBudget/crop[swK]",CA);
    Input(canopyReflectivity).imports("energyBudget/crop[swReflectivityTop]",CA);
    Input(Pgmax).imports("./lightResponse[Pgmax]",CA);
    Input(lue).imports("./lightResponse[lue]",CA);
    Input(RdLeaf).imports("./lightResponse[Rd]",CA);

    Output(Pn).help("Net assimilation rate").unit("g CO2/ground m2/h");
    Output(Pg).help("Gross assimilation rate").unit("g CO2/ground m2/h");
    Output(Rd).help("Dark respiration rate").unit("g CO2/ground m2/h");
}

void CanopyPhotosynthesis::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./lightResponse"))
        builder.
        box("vg::LeafLightResponse").name("lightResponse").
        endbox();
}

void CanopyPhotosynthesis::update() {
    // From Goudriaan & van Laar (1994) but scaled to parAbsorbed,
    // i.e. the total PAR sbcorbed by the canopy has been computed elsewhere

    // Compute proportionality to achieve parAbsorbed
    double proportionality(0);
    for (int i= 0; i<3; ++i) {
        double laic = xGauss3[i]*lai;
        proportionality += (1.-canopyReflectivity)*k*exp(-k*laic)*lai*wGauss3[i];
    }
    proportionality = parAbsorbed/proportionality;

    // Integrate photosynthesis
    Pg = parAbsorbed = 0.;
    for (int i= 0; i<3; ++i) {
        double
            laic = xGauss3[i]*lai,
            absorbed = proportionality*(1.-canopyReflectivity)*k*exp(-k*laic)*lai,
            photosynthesis = (Pgmax > 0) ? Pgmax*(1.-exp(-absorbed*lue/Pgmax)) : 0.;
        Pg += wGauss3[i]*photosynthesis;
        parAbsorbed += wGauss3[i]*absorbed;
    }
    Pg *= lai;
    Rd = RdLeaf*lai;
    // Convert to g CO2/m2/h]
    Pg *= 3.6;
    Rd *= 3.6;
    Pn = Pg - Rd;
}

} //namespace

