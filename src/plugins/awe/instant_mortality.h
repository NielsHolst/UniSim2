/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSTANT_MORTALITY_H
#define INSTANT_MORTALITY_H

#include <QDate>
#include <base/box.h>

namespace awe {

class PlantGrowthStage;

class InstantMortality : public base::Box
{
public:
    InstantMortality(QString name, QObject *parent=0);
    void update();
	
private:
    // inputs
    QDate date, today;
    double seedlingMortality, juvenileMortality, matureMortality, seedsOnPlantMortality;
    // outputs
    double seedlingMortalityToday, juvenileMortalityToday, matureMortalityToday, seedsOnPlantMortalityToday;
};


}
#endif
