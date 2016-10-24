/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_volatilization.h"
#include <cmath>
#include <base/publish.h>

using namespace std;

using namespace base;

namespace PestTox {
	
PUBLISH(leafvolatilization)

leafvolatilization::leafvolatilization(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(VP).equals(5.73e-8);    //vapor pressure of the pesticide (Pa) at 25 degC
    Input(Ea).equals(95.);        //activation energy in kJ/mol (default value)
    Input(R).equals( 8.3145);      //gas constant in J K-1 mol-1
    Input(Tr).equals(25.);        //reference temperature in degC
    Input(Ta).equals(25.);        //air temperature at the time of pesticide application, degC
    Input(Doseappl).equals(3.6);  //application rate (kg/ha)automatically converted to microgram/m2

    Output(ln_ER);         //evaporation rate in microgram/m2/h (in C++ log is "ln", and log10 is "log")
    Output(kTr);           //rate constant of vaporization at reference temperature, (day-1) at 25 deg C
    Output(ER);            //evaporation rate (microgram/m2/day)
    Output(fv);            //fraction of pesticide emitted in 1 day
    Output(fr);            //Fraction remaining on leaf -or-fraction of pesticide reaching the leave still there in 1 day
    Output(klv);           //rate constant of vaporization at actual temperature, (day-1)

}

void leafvolatilization::reset() {

    ln_ER = 0.;
    ER = 0.;
    kTr = 0.;
    klv = 0.;
    fv = 0.;
    fr = 0.;

}

void leafvolatilization::update() {

    double x = (VP > 0) ? log(VP) : 0;
    ln_ER = 12.2 + 0.933 * x;
    ER = 24. * exp(ln_ER);
    fv = (ER/(Doseappl*(1e9/1e4)) <1) ? ER/(Doseappl*(1e9/1e4)) : 0.99;
    fr = 1. - fv;
    kTr = -log(fr);
    klv = kTr * exp(((Ea*1000.)/R)*((1./(Tr + 273.15)) - (1./(Ta + 273.14))));

  }

} //namespace

