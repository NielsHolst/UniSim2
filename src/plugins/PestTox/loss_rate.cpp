/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "loss_rate.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {
	
LossRate::LossRate(QString name, QObject *parent)
	: Box(name, parent)
{
    Output(instantaneous).help("Instantaneous loss rate (s-1)");
    Output(finite).help("Finite loss rate (d-1)");
}

void LossRate::reset() {
    update();
}

void LossRate::update() {
    instantaneous = computeInstantaneous();
    finite = 1. - exp(-24*60*60*instantaneous);
}

} //namespace

