/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "organic_carbon_water_partitioning.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (OrganicCarbonWaterPartitioning)
	
OrganicCarbonWaterPartitioning::OrganicCarbonWaterPartitioning(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Koc).equals(9.);    //can varry from 9 - 60,000 L/kg
    Input(pH).equals(7.);
    Input(pKa).equals(0.8);   //The acid dissociation constants for glyphosate are pKa1 0.8 (1st phosphonic), pKa2 2.3 (carboxylate), pKa3 6.0 (2nd phosphonic), and pKa4 11.0 (amine).

    Output(KocpH);

}

void OrganicCarbonWaterPartitioning::update() {

    KocpH = Koc * pow(10, -pH)/(pow(10, -pH)+ pow(10, -pKa));

}

} //namespace

