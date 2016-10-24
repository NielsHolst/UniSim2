/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_overall_rate_constant.h"
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (leafoverallrateconstant)
	
leafoverallrateconstant::leafoverallrateconstant(QString name, QObject *parent)
	: Box(name, parent)
{

    Input(klu).equals(0.);       //rate constant of leaf uptake (per day)
    Input(klv).equals(0.);       //rate constant of leaf volatilization (per day)
    Input(kld).equals(0.);       //rate constant of leaf degradation (per day)

    Output(kov);          //overall rate constant of leaf removal processes (per day)

}

void leafoverallrateconstant::reset() {

    kov = 0.;
}

void leafoverallrateconstant::update() {

    kov = klv + klu + kld;    

}

} //namespace

