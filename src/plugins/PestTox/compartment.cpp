/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "compartment.h"
#include <base/any_year.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace PestTox {

PUBLISH(Compartment)
	
Compartment::Compartment(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(gain).help("Daily gain (g a.i. per ha)");
    Input(loss).help("Daily loss (g a.i. per ha)");

    Output(load).help("Current pesticide load (g a.i. per ha)");
}

void Compartment::update() {

    load += gain - loss;
    TestNum::snapToZero(load, 1e-18);
    if (load < 0.)
        ThrowException("Load cannot be negative").value(load).context(this);
}

} //namespace

