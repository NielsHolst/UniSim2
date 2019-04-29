#include <base/publish.h>
#include "cadaver_conversion.h"
#include <base/vector_op.h>

using vector_op::sum;
using namespace base;

namespace aphid {

PUBLISH(CadaverConversion)

CadaverConversion::CadaverConversion(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the number of cadaver units based on the life stage of the aphid when becoming a cadaver");
    Input(succumbedApterousNymphs).help("New apterous nymph cadavers").unit("per tiller");
    Input(succumbedAlateNymphs).help("New alate nymph cadavers").unit("per tiller");
    Input(succumbedApterousAdult).help("New apterous adult cadavers").unit("per tiller");
    Input(succumbedAlateAdults).help("New alate adult cadavers").unit("per tiller");
    Output(cadavers).help("Cadavers in standardised units").unit("per tiller");
}

void CadaverConversion::update() {
    // The inputs come as vectors which hold the age-distribution of how far the individuals came
    // in their development as aphids before turning into cadavers. That age distribution becomes
    // irrelevant once they have become cadaders.
    double nymphCadavars = 0.5*(sum(succumbedApterousNymphs) + sum(succumbedAlateNymphs)),
           alateAdultCadavers = 0.8*sum(succumbedAlateAdults);
    cadavers = nymphCadavars + alateAdultCadavers + sum(succumbedApterousAdult);
    // Hemmati et al 2001: winged S. avenae produce 20% less spores than apterous adult.
    // let's assume that nymphs produce on average 50% less spores than apterous adult.
}

}


