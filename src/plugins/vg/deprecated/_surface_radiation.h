/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SURFACE_RADIATION_H
#define SURFACE_RADIATION_H
#include <QObject>
#include <QString>

namespace vg {

struct SurfaceRadiation {
    SurfaceRadiation(const QObject *parent);

    const QObject *_parent;

    struct Spectrum {
        Spectrum(double tra_=1) : tra(tra_) {}
        Spectrum& operator*=(const Spectrum &s2);

        double tra;

        struct Direction {
            Direction() : abs(0), ref(0) {}
            void setRef(double tra);
            void setAbs(double tra);
            double abs, ref;
            void check(double tra, QString name, const QObject *context);
        } inner, outer;
        void check(QString name, const QObject *context);
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
