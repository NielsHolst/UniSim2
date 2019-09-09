/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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

