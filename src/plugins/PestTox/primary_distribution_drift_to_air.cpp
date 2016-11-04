/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primary_distribution_drift_to_air.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (PrimaryDistributionToAir)
	
PrimaryDistributionToAir::PrimaryDistributionToAir(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(Nd).equals(0.2);         //meter (m)
    Input(W).equals(500.);         //m
    Input(Pnsz).equals(0.);        //m
    Input(Doseappl).equals(1.);    //kg a.i/ha
    Input(A0).equals(39.);
    Input(A1).equals(0.90);
    Input(B0).equals(2.28);
    Input(B1).equals(0.147);
    Input(X0).equals(114);
    Input(Y0).equals(1.29);

    Output(Xpass);           //m
    Output(fXpass);
    Output(Dosedrift);       //kg a.i/ha
    Output(fd);

}


void PrimaryDistributionToAir::update() {

    int npass = int((W -(2.*Pnsz))/Nd);
    fXpass = 0;
    for(int i = 1; i <= npass; ++i){
        Xpass = Pnsz + (Nd * (i - 0.5));     //"i" is the "pass", "losscurve" is the fraction lost at distance Xpass (m)
        double losscurve = (A0*exp(-Xpass/B0) + A1*exp(-Xpass/B1) + X0*pow(Xpass, Y0))*0.01;
        if (losscurve > 1)
            fXpass += 1;
        else if   (losscurve < 0)
            fXpass += 0;
        else
            fXpass += losscurve;
    }
    fd = fXpass/npass;
    Dosedrift = Doseappl * fd;
}

} //namespace

