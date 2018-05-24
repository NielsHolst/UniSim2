#include <base/publish.h>
#include "cadavers_units2.h"
#include <base/vector_op.h>
using vector_op::sum;
using vector_op::product;
using namespace base;

namespace aphid {

PUBLISH(cadavers_units2)

cadavers_units2::cadavers_units2(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_cadavers_adults);
    Input (nb_cadavers_nymphs);
    Input (nb_cadavers_W_nymphs);
    Input (nb_cadavers_alates);
    //Input (nb_cadavers_N_A);
    Output(nb_units);
}

void cadavers_units2::reset() {
    resultN=
    resultN_tot=
    resultW=
    result_adult=
    nb_units=QVector <double>();
}

void cadavers_units2::update() {
    sum(resultN_tot,nb_cadavers_nymphs, nb_cadavers_W_nymphs);
    product(resultN,resultN_tot,0.5); // give the vector result by multiplying the vector
            //exposed_nymphs by 0.5
    product(resultW,nb_cadavers_alates,0.8);
    sum(result_adult,resultW,nb_cadavers_adults);
    sum(nb_units, result_adult,resultN);
    // Hemmati et al 2001: winges SA produce 20% less spores than apterous adult.
    // let's assume for now that nymph produce on average 50% less spores than apterous adult.
}


}
