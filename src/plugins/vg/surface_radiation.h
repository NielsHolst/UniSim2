/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SURFACE_RADIATION_H
#define SURFACE_RADIATION_H
#include <QString>

namespace vg {

struct SurfaceRadiation {
    SurfaceRadiation();

    struct Spectrum {
        Spectrum(double tra_=1) : tra(tra_) {}
        Spectrum& operator*=(const Spectrum &s2);

        double tra;

        struct Direction {
            Direction() : abs(0), ref(0) {}
            void setRef(double tra);
            void setAbs(double tra);
            double abs, ref;
            void check(double tra, QString name);
        } inner, outer;
        void check(QString name);
    } light, directLight, lw;

    // Methods
    void setToZero();
    SurfaceRadiation& asCover(double transmissivity, double directTransmissivity, double absorptivity, double emissivity);
    SurfaceRadiation& asScreen(double transmissivity, double absorptivityLwOuter, double absorptivityLwInner);
    void check();

    // Operators
    SurfaceRadiation& operator*=(const SurfaceRadiation &s2);
};

} //namespace


#endif
