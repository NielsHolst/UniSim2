/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "phenology.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(Phenology)

Phenology::Phenology(QString name, QObject *parent)
    : Box(name, parent) {
    help("models coffee plant development");
    Input(day).imports("sim[step]");
    Input(date).imports("calendar[date]");
    Input(delT).imports("calendar[timeStepDays]");
    Input(rain).imports("weather[Rain]");
    Input(Tshade).imports("Tavg[value]");

    Input(rainDoyHi).equals(1000.).unit("mm d").help("Flower trigger");
    Input(TMatB).equals(10).unit("").help("Base temperature for maturation");
    Input(TMatT).equals(2780).unit("").help("Thermal time to maturation");
    Input(DaysPlnOp).equals(900).unit("d").help("Days between start and full productivity");
//    Input(sinkPMax).equals(3.6).unit("-").help("Sink strength for storage organs");
//    Input(kSinkPMax).equals(0.8).unit("").help("");
//    Input(SINKP).equals().unit("").help("");

    Output(DVS).unit("-").help("Development stage");
    Output(sensit).unit("-").help("Sensitivity to rain");
    Output(dayFill).unit("bool").help("Are beans filling?");
    Output(dSensit).unit("").help();
    Output(rainDoy).unit("").help();
    Output(dayFl).unit("").help();
    Output(dDVSMAX).unit("").help();
    Output(dDVS).unit("").help();
    Output(rDVS).unit("").help("? Development resetting");
//    Output(SINKPMAXnew).unit("").help();
}

void Phenology::reset() {
}

void Phenology::update() {
    // Determine whether beans are filling
    dayFill = (DVS > 0. && DVS < 1.);

    // Make the crop sensitive to rainfall in the beginning of the year
    int doy = date.dayOfYear();
    bool isNewYear = (doy==365);
    if (dayFill)
      dSensit = -sensit/delT;
    else if (isNewYear && day > DaysPlnOp)   // accumulated days of plant life
      dSensit = 1.;
    else
      dSensit = 0.;

    // Trigger flowering
    rainDoy = rain*doy;
    dayFl = (DVS == 0 && rainDoy > rainDoyHi) ? sensit : 0.;

    // Compute development rate
    dDVSMAX = (dayFill || dayFl == 1.) ? max(0., min(1., (Tshade - TMatB)/ TMatT)) : 0.;
    dDVS = (doy > 360 && dDVSMAX > 0.) ? (1.-DVS)/delT : dDVSMAX;

    // Compute development retardation
    if (DVS >= 1.) {
      rDVS = DVS/delT;
//      SINKPMAXnew = sinkPMax - kSinkPMax * SINKP;
    }
    else {
      rDVS = 0.;
    }

    // Update state
    DVS += dDVS - rDVS;
    sensit += dSensit;
}

} //namespace

