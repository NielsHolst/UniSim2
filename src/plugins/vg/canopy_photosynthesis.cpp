/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "canopy_photosynthesis.h"

using namespace base;

namespace vg {
	
PUBLISH(CanopyPhotosynthesis)

CanopyPhotosynthesis::CanopyPhotosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes canopy photosynthetic rate");
    Input(lai).imports("/*/crop[lai]",CA);
    Input(k).imports("energyBudget/crop[swK]",CA);
    Input(leafAn).imports("./*/leafPhotosynthesis[An]");
    Input(leafAg).imports("./*/leafPhotosynthesis[Ag]");
    Output(An).help("Net canopy assimilation rate").unit("μmol CO2/m2 leaf/s");
    Output(Ag).help("Gross canopy assimilation rate").unit("μmol CO2/m2 leaf/s");
    Output(Ar).help("Canopy respiration rate").unit("μmol CO2/m2 leaf/s");
    Output(Pn).help("Net canopy assimilation rate").unit("g CO2/ground m2/h");
    Output(Pg).help("Gross canopy assimilation rate").unit("g CO2/ground m2/h");
    Output(Pr).help("Canopy respiration rate").unit("g CO2/ground m2/h");
    Output(PnLeaf).help("Net canopy assimilation rate").unit("g CO2/leaf m2/h");
    Output(PgLeaf).help("Gross canopy assimilation rate").unit("g CO2/leaf m2/h");
    Output(PrLeaf).help("Canopy respiration rate").unit("g CO2/leaf m2/h");
}

void CanopyPhotosynthesis::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./top"))
        builder.
        box().name("top").
            box("IntercellularCo2").name("intercellularCo2").endbox().
            box("LeafParAbsorbed").name("parAbsorbed").endbox().
            box("LeafRespiration").name("respiration").endbox().
            box("LeafPhotosynthesisAj").name("Aj").endbox().
            box("StomatalResistanceBallBerry").name("stomatalResistance").endbox().
            box("LeafPhotosynthesis").name("leafPhotosynthesis").endbox().
        endbox().
        box().name("mid").
            box("IntercellularCo2").name("intercellularCo2").endbox().
            box("LeafParAbsorbed").name("parAbsorbed").endbox().
            box("LeafRespiration").name("respiration").endbox().
            box("LeafPhotosynthesisAj").name("Aj").endbox().
            box("StomatalResistanceBallBerry").name("stomatalResistance").endbox().
            box("LeafPhotosynthesis").name("leafPhotosynthesis").endbox().
        endbox().
        box().name("bottom").
            box("IntercellularCo2").name("intercellularCo2").endbox().
            box("LeafParAbsorbed").name("parAbsorbed").endbox().
            box("LeafRespiration").name("respiration").endbox().
            box("LeafPhotosynthesisAj").name("Aj").endbox().
            box("StomatalResistanceBallBerry").name("stomatalResistance").endbox().
            box("LeafPhotosynthesis").name("leafPhotosynthesis").endbox().
        endbox();
}

void CanopyPhotosynthesis::update() {
    // μmol CO2/m2 leaf/s
    An = vector_op::sum(leafAn);
    Ag = vector_op::sum(leafAg);
    Ar = Ag - An;
    // μmol CO2/m2 ground/s
    Pn = An*lai;
    Pg = Ag*lai;
    Pr = Ar*lai;
    // g CO2/ground m2/h
    Pn *= 3600*44.01*1e-6;
    Pg *= 3600*44.01*1e-6;
    Pr *= 3600*44.01*1e-6;
    // g CO2/leaf m2/h
    PnLeaf = Pn*lai;
    PgLeaf = Pg*lai;
    PrLeaf = Pr*lai;
}

} //namespace

