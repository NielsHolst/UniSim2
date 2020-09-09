/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_PHOTOSYNTHESIS_H
#define LEAF_PHOTOSYNTHESIS_H

#include <base/box.h>

namespace vg {

class LeafPhotosynthesis : public base::Box
{
public:
    LeafPhotosynthesis(QString name, QObject *parent);
    void amend();
    void update();
private:
    // Inputs
    double
        parAbsorbed,
        lai, k,
        canopyReflectivity,
        Pgmax, lue, RdLeaf;
    // Outputs
    double Pg, Pn, Rd;
};
} //namespace


#endif
