/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fish.h"

using namespace base;

namespace student {

PUBLISH(Fish)

Fish::Fish(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initialNumber).equals(10).unit("Total number");
    Input(initialWeight).unit("kg");
    Input(death);
    Input(timeStepDays);
    Input(dayOfYear);
    Output(number);
    Output(totalWeight);
    Output(individuals);
}

void Fish::initialize() {
    individuals.resize(initialNumber);
}

void Fish::reset() {
    currentDayOfYear = 0;
    number = initialNumber;
    totalWeight = 0.;
    for (int i=0; i<initialNumber; ++i) {
        port("initialWeight")->update();
        totalWeight += individuals[i] = initialWeight;
    }
}

void Fish::update() {
    // Mortality
    if (currentDayOfYear != dayOfYear) {
        effectuateMortality();
        currentDayOfYear = dayOfYear;
    }
    // Growth
    totalWeight = 0.;
    for (int i=0; i<initialNumber; ++i) {
        double &weight(individuals[i]);
        double feed = weight*growthRate(weight)*timeStepDays;
        totalWeight += individuals[i] += feed*conversionEfficiency(weight);
    }
}

double Fish::conversionEfficiency(double weight) {
    return 1./(0.2014*weight + 0.7888);
}

double Fish::growthRate(double weight) {
    // Relative growth rate per day
    return (weight > 0.) ? (-0.67254091*log(weight) + 1.09924419)/100. : 0.;
}

void Fish::effectuateMortality() {
    for (int i=0; i<initialNumber; ++i) {
        port("death")->update();
        if (death) {
            if (individuals[i] > 0.) {
                individuals[i] = 0;
                --number;
            }
            // To do: Else we should kill another fish
        }
    }
}
}
