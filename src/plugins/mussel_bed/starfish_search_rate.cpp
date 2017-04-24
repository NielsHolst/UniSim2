/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera [antonio.aguera@gmail.com].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH SEARCH RATE MODEL: determines the apparency or search rate for starfish to be considered in the functional response. The empirical value of 0.17
** from Barbeau and Wong is evaluated against field current environmental conditions that difficult starfish activity and movement*/

#include "starfish_search_rate.h"

using namespace UniSim;

namespace mussel_bed {

StarfishSearchRate::StarfishSearchRate(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("s", &s, 0.17,this, "maximum apparency under optimal conditions" );
    new Variable<double>("value", &value, this, "apparency of prey under current stress conditions");
}

void StarfishSearchRate::initialize() {
    scales = seekChildren<Model*>("stscale");
}

void StarfishSearchRate::reset() {
    value = 1;
    }

void StarfishSearchRate::update() {

    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->pullValue<double>("value");
    }
    value = value*s;
}

} //namespace
