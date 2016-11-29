/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "macropore_flow.h"
#include <cmath>
#include <base/publish.h>



using namespace base;

namespace PestTox {

PUBLISH (MacroporeFlow)
	
MacroporeFlow::MacroporeFlow(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(fsand).equals(0.25);
    Input(fsilt).equals(0.25);
    Input(fclay).equals(0.5);
    Input(fa).equals(0.25);           //fraction of air in the soil
    Input(fw).equals(0.25);           //fraction of water in the soil
    Input(fmacropore).equals(0.3);    //fraction of macropore i.e. pore fractions which acts as macropores
    Input(Flowrate).equals(0.35);
    Input(frr).equals(0.5);           //fractionrainrunoff
    Input(tpevent).equals(5.);        //number of hours precipitation occurs on a rainy day
    Input(P).equals(1.);              //average  daily rainfall per rainfall event in a given month (mm)
    Input(frsw).equals(0.);           //fraction of pesticide in soil water
    Input(Tf).equals(0.);            //tillage factor
    Input(Doseaflm).equals(0.);

    Output(fporemobile);       //mobile pore fraction in topsoil
    Output(Vporeimmobile);     //immobile pore volume in topsoil
    Output(C);                 //one hour storage capacity
    Output(Cused);             //used capacity
    Output(Cfree);
    Output(Pcapacitymm);       //mm rain capacity or freecapacity recalculated as the number of mm rain that can be taken up in 1 hour (mm)
    Output(Preqh);             //rainfall required for macropore flow per hour
    Output(Preqday);           //rainfall required for macropore flow per day
    Output(Pi);                //rainfall distribution integrated
    Output(fmp);
    Output(Vporemobile);       //mobile pore volume in topsoil

}

void MacroporeFlow::update() {

    fporemobile = 0.72 * fsand + 0.35 * fsilt + 0.14 * fclay;
    Vporemobile = fporemobile * (fw * fa);
    Vporeimmobile = (fw + fa)- Vporemobile;
    C = Vporeimmobile * 0.01 + ((1. - fmacropore) * Vporemobile *Flowrate/24.);
    Cused = (fw /(fw + fa))* C;
    Cfree = C - Cused;
    Pcapacitymm = 1000. * Cfree;
    Preqh = Pcapacitymm /(1. - frr);
    Preqday = Preqh * 24.;
    Pi = 1.-(1.- exp((-tpevent /(24.*P)*Preqday)));
    fmp = Pi*frsw*(Tf/7.5);
}

} //namespace

