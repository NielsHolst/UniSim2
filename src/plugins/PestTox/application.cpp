/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "application.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH(application)
	
application::application(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(day).equals(1);          //day of application (day)
    Input(ae).equals(360.);     //active ingredient (g a.e/L)
    Input(ar).equals(6.25);     //application rate (L/ha)

    Output(Doseappl);    //Take the value of the dose applied on the day, otherwise zero (kg a.i/ha)
}

void application::reset() {
    daysPassed = 0;
    Doseappl = 0;
}

void application::update() {
    ++daysPassed;
    Doseappl = (daysPassed == day) ? ((ae * ar)/(0.75*1000.)) : 0.;
}

} //namespace

