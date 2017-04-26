/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH SEARCH RATE MODEL: determines the apparency or search rate for starfish to be considered in the functional response. The empirical value of 0.17
** from Barbeau and Wong is evaluated against field current environmental conditions that difficult starfish activity and movement*/

#include "starfish_search_rate.h"

using namespace base;

namespace MusselBed {

StarfishSearchRate::StarfishSearchRate(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(s).equals(0.17).help("maximum apparency under optimal conditions");
    Output(value).help("apparency of prey under current stress conditions");
}

void StarfishSearchRate::initialize() {
    scales = findMany<Box>("stscale");
}

void StarfishSearchRate::reset() {
//    value = 1;        //NH
    value = 0;          //NH
}

void StarfishSearchRate::update() {
    value = s; //NH oops!
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->port("value")->value<double>();
    }
//    value = value*s;  // NH
}

} //namespace
