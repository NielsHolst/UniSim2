/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_photo_degradation.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (leafphotodegradation)
	
leafphotodegradation::leafphotodegradation(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(I).equals(10.);             //light intensity (Wh m-2 day-1)
    Input(kOH).equals(0.0003);        //cm3/(molecules*s)
    Input(dl).equals(12);             // hours (h)

    Output(log_OH);            //log10(OH), log (molecules/cm3)
    Output(kld);               // day-1
    Output(concentration);     //kg a.i/ha
    Output(outflow);           //fraction removed by degradation
}

void leafphotodegradation::reset() {
    concentration = 0.;
    outflow = 0.;
    log_OH = 0.;
    kld = 0.;
}

void leafphotodegradation::update() {

    log_OH = (0.0003 * I) + 4.7783;
    kld = 3600.*24.*kOH*pow(10, (log_OH * dl/24.));

}

} //namespace

