/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "application.h"
#include <base/any_year.h>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH(Application)
	
Application::Application(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(date).imports("calendar[date]");
    Input(applicationDate).equals("1/3/*").help("Date of application");
    Input(concentration).equals(480.).help("Concentration (g a.i/L)");
    Input(rate).equals(3.125).help("Application rate (L/ha)");

    Output(dose).help("Dose applied today (g a.i. per ha");
}

void Application::update() {

    dose = equals(date, applicationDate) ? concentration*rate : 0.;
}

} //namespace

