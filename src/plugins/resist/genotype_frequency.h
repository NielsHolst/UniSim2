/* Copyright 2017-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GENOTYPE_FREQUENCY_H
#define GENOTYPE_FREQUENCY_H
#include <base/box.h>

namespace resist {

class GenotypeFrequency : public base::Box
{
public:
    GenotypeFrequency(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double N, Ntotal;
    // Outputs
    double value;
};

}

#endif
