/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    Input(doseApplied).imports("applications[dose]").help("Dose of pesticide (kg a.i/ha)");
    Output(doseTaken).help("Dose taken by this compartment (kg a.i/ha)");
    Output(fractionTaken).help("Fraction of applied dose taken by this compartment [0;1]");
}


} //namespace

