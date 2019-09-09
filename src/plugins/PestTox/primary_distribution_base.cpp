/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "primary_distribution_base.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PrimaryDistributionBase::PrimaryDistributionBase(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(fieldArea).imports("field[area]").help("Area of whole field (m2)");
    Input(doseApplied).imports("applications[dose]").help("Dose of pesticide (g a.i/ha)");
    Output(doseTaken).help("Dose taken by this compartment (g a.i/ha)");
    Output(fractionTaken).help("Fraction of applied dose taken by this compartment [0;1]");
}


} //namespace

