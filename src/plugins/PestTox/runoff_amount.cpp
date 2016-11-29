/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "runoff_amount.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (RunoffAmount)
	
RunoffAmount::RunoffAmount(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(P).equals(1.);     //average  daily rainfall per rainfall event in a given month (mm)
    Input(fsand).equals(0.5);

    Output(Q);

}

void RunoffAmount::update() {

//  x is for loamy soils and y is for sandy soils


    double x = ((-0.061108 - 0.0041626*P + 0.0040395*pow(P, 2) - 9.0361e-6*pow(P, 3)) < 0) ?
               (-0.0041626*P + 0.0040395*pow(P, 2) - 9.0361e-6*pow(P, 3)) :
               (-0.061108 - 0.0041626*P + 0.0040395*pow(P, 2) - 9.0361e-6*pow(P, 3));
    double y = ((-0.016427 - 0.011377*P + 0.0026284*pow(P, 2) - 5.8564e-6*pow(P, 3)) < 0) ?
               (-0.011377*P + 0.0026284*pow(P, 2) - 5.8564e-6*pow(P, 3)) :
               (-0.016427 - 0.011377*P + 0.0026284*pow(P, 2) - 5.8564e-6*pow(P, 3));
    double z = (fsand < 0.5) ? x : y;
    Q = (z > 0) ? z : 0;

//   These codes are also OK

//    double x = -0.0041626*P + 0.0040395*pow(P, 2) - 9.0361e-6*pow(P, 3);
//    double y = -0.011377*P + 0.0026284*pow(P, 2) - 5.8564e-6*pow(P, 3);
//    Q = (fsand < 0.5) ? x : y;  // x is for loamy soils, and y for sandy soils

}

} //namespace

