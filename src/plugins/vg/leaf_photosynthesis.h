/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_LEAF_PHOTOSYNTHESIS_H
#define VG_LEAF_PHOTOSYNTHESIS_H

#include <QPair>
#include <base/box.h>

namespace vg {

class LeafPhotosynthesis : public base::Box
{
public:
    LeafPhotosynthesis(QString name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Inputs
    double
        sunlightPhotonCoef,
        kDiffuse, kDirect, kDirectDirect, scattering,
        diffuseReflectivity, directReflectivity,
        parDiffuse, parDirect,
        Pgmax, LUE,
        xGauss, wGauss, lai, RdLeaf, sinb;

    // Outputs
    double absorptivity, parAbsorbed, Pg, Pn, Rd;

    // Data
    double _parDiffuseW, _parDirectW;

    // Methods
    double lad() const;
    double laic() const;
    double absorbedByShadedLeaves() const;
    QPair<double,double> absorbedBySunlitLeaves(double absorbedShaded) const;
    double grossAssimilation(double absorbed) const;
};
} //namespace


#endif
