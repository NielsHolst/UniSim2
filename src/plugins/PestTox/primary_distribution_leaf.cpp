/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primary_distribution_leaf.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (primarydistributionleaf)
	
primarydistributionleaf::primarydistributionleaf(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Doseappl).equals(1.);         //applied dosage kg a.i/ha
    Input(Dosedrift).equals(1.);
    Input(stage1).equals(0.);            //leaf development
    Input(stage2).equals(0.);            //tillering
    Input(stage3).equals(0.);            //stem elongation
    Input(stage4).equals(0.);            //senescence

    Output(Doserl);
    Output(fi);                   //fraction intercepted

}

void primarydistributionleaf::reset() {

    Doserl = 0.;
    fi = 0.;

}

void primarydistributionleaf::update() {

    fi = 0.25*stage1 + 0.5*stage2 + 0.7*stage3 + 0.9*stage4;
    Doserl = (Doseappl - Dosedrift) * fi;

}

} //namespace

