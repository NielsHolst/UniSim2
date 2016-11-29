/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "soil_structure.h"
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace PestTox {

PUBLISH (SoilStructure)
	
SoilStructure::SoilStructure(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(fa).equals(0.25);     //fraction of air in the soil
    Input(fw).equals(0.25);     //fraction of water in the soil

    Output(fs);
    Output(porosity);

}

void SoilStructure::reset() {

    update();

}

void SoilStructure::update() {

    porosity = fa + fw;
    fs = 1. - porosity;

    double sum = fa+fw+fs;
    if (TestNum::ne(sum, 1.))
        ThrowException("Sum of soil fractions must equal 1").value(sum).context(this);

}

} //namespace

