/* Copyright 2005-2019 by
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
#include "leaf_photosynthesis.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(LeafPhotosynthesis)

LeafPhotosynthesis::LeafPhotosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes light capture and photosynthetic rate");
    Input(par).imports("indoors/light[parTotal]",CA);
    Input(lai).imports("crop[lai]",CA);
    Input(k).imports("radiationLayers/crop[swK]",CA);
    Input(canopyReflectivity).imports("radiationLayers/crop[swReflectivityUp]",CA);
    Input(Pgmax).imports("./lightResponse[Pgmax]",CA);
    Input(lue).imports("./lightResponse[lue]",CA);
    Input(RdLeaf).imports("./lightResponse[Rd]",CA);

    Output(Pn).help("Net assimilation rate").unit("g CO2/ground m2/h");
    Output(Pg).help("Gross assimilation rate").unit("g CO2/ground m2/h");
    Output(Rd).help("Dark respiration rate").unit("g CO2/ground m2/h");
}

void LeafPhotosynthesis::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./lightResponse"))
        builder.
        box("vg::LeafLightResponse").name("lightResponse").
        endbox();
}

void LeafPhotosynthesis::update() {
    // From Goudriaan & van Laar (1994)
    Pg = 0.;
    for (int i= 0; i<3; ++i) {
        double
            laic = xGauss3[i]*lai,
            absorbed = par*(1.-canopyReflectivity)*k*exp(-k*laic)*lai,
            photosynthesis = (Pgmax > 0) ? Pgmax*(1.-exp(-absorbed*lue/Pgmax)) : 0.;
        Pg += wGauss3[i]*photosynthesis;
    }
    Pg *= lai;
    Rd = RdLeaf*lai;
    // Convert to g CO2/m2/h]
    Pg *= 3.6;
    Rd *= 3.6;
    Pn = Pg - Rd;
}

} //namespace

