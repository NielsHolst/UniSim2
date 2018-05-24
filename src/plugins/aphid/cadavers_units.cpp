#include <base/publish.h>
#include "cadavers_units.h"
#include <base/vector_op.h>
using vector_op::sum;
using vector_op::product;
using namespace base;

namespace aphid {

PUBLISH(cadavers_units)

cadavers_units::cadavers_units(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_cadavers_adults);
    Input (nb_cadavers_nymphs);
    //Input (nb_cadavers_alates);
    //Input (nb_cadavers_N_A);
    Output(nb_units);
}

void cadavers_units::reset() {
    //result2=
    nb_units=
    result1=QVector <double>();
}

void cadavers_units::update() {
    product(result1,nb_cadavers_nymphs,0.5); // give the vector result by multiplying the vector
            //exposed_nymphs by 0.5
    //sum(result2,nb_cadavers_N_A,nb_cadavers_adults);
    sum(nb_units, nb_cadavers_adults,result1);
    // Hemmati et al 2001: winges SA produce 20% less spores than apterous adult.
    // let's assume for now that nymph produce on average 50% less spores than apterous adult.
}


}
