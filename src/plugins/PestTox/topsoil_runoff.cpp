/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoil_runoff.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (TopSoilRunoff)
	
TopSoilRunoff::TopSoilRunoff(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(S).equals(0.01);
    Input(wbz).equals(3.3);
    Input(Doseaflrm).equals(0.);      //kg a.i/ha
    Input(Doseldw).equals(0.);        //dose leaft on the leaf on beginning rainfall will be washed to the soil
    Input(P).equals(1.);              //average  daily rainfall per rainfall event in a given month (mm)
    Input(KocpH).equals(5.678e-6);
    Input(fom).equals(0.09);          //organice matter content in the soil
    Input(Q).equals(5.);

    Output(F);
    Output(Fslope);
    Output(Fbuffer);
    Output(fsr);
}

void TopSoilRunoff::update() {

    if (S > 0.2){
        Fslope = 1;
    }
    else if (S <0.2){
        Fslope = 0.02153*S + 0.001423*pow(S, 2);
    }
    else{
        Fslope = 0.02153 + 0.0014426;
    }
    Fbuffer = pow(0.83, wbz);
    F = Fbuffer * Fslope;
    Kd = KocpH * fom;
    fsr = (P > 0) ? (Q/P) * F * (100./ 1. + Kd) : 0;

}

} //namespace

