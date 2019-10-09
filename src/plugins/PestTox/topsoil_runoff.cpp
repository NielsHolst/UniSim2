/* Copyright 2016-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "topsoil_runoff.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (TopsoilRunoff)
	
TopsoilRunoff::TopsoilRunoff(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(S).equals(0.01);
    Input(wbz).equals(1.);
    Input(P).equals(1.);              //average  daily rainfall per rainfall event in a given month (mm)
    Input(KocpH).equals(5.678e-6);
    Input(fom).equals(0.09);          //organice matter content in the soil
    Input(Q).equals(5.);

    Output(F);
    Output(Fslope);
    Output(Fbuffer);
    Output(fsr);
}

void TopsoilRunoff::update() {
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
    fsr = (P > 0) ? (Q/P) * F * (1./ (1. + Kd)) : 0;
}

} //namespace

