/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <math.h>
#include <qglobal.h>
#include <QString>
#include <base/exception.h>
#include <base/test_num.h>
#include "general.h"
#include "surface_radiation.h"

using namespace base;

namespace vg {

/*! \class SurfaceRadiation
 * \brief Basic radiation characteristics of a surface
 *
 * This class manages the absortivity, emissivity, reflectivity and transmissivity of a surface.
 * Short-waved diffuse (_light_) and direct radiation (_directLight_), and long-waved radiation (_lw_) are considered separately.
 *
*/
SurfaceRadiation::SurfaceRadiation() {
    light =
    directLight =
    lw = Spectrum(1);
}

//! Initialize from cover characteristics
SurfaceRadiation& SurfaceRadiation::asCover(double transmissivity, double directTransmissivity,
                                            double absorptivity, double emissivity) {
    light.tra = transmissivity;
    directLight.tra = directTransmissivity;
    lw.tra = 0.;

    light.outer.abs =
    light.inner.abs =
    directLight.outer.abs =
    directLight.inner.abs = absorptivity;
    lw.outer.abs =
    lw.inner.abs = emissivity;

    light.outer.setRef(light.tra);
    light.inner.setRef(light.tra);
    directLight.outer.setRef(directLight.tra);
    directLight.inner.setRef(directLight.tra);
    lw.outer.setRef(lw.tra);
    lw.inner.setRef(lw.tra);
    check();
    return *this;
}

//! Initialize from screen characteristics
SurfaceRadiation& SurfaceRadiation::asScreen(double transmissivity, double absorptivityLwOuter, double absorptivityLwInner) {
    light.tra = transmissivity;
    lw.tra = 0.;
    light.outer.abs =
    lw.outer.abs = absorptivityLwOuter;
    light.outer.setRef(light.tra);
    lw.outer.setRef(lw.tra);

    lw.inner.abs = absorptivityLwInner;
    lw.inner.setRef(lw.tra);
    light.inner.ref = std::min(lw.inner.ref, 1. - light.tra);
    light.inner.setAbs(light.tra);

    directLight = light;
    check();
    return *this;
}

//! Set transmissivity to 1
void SurfaceRadiation::setToZero() {
    light =
    directLight =
    lw = Spectrum(0);
}

//! Set reflectivity from transmissivity (assumes absorptivity already set)
void SurfaceRadiation::Spectrum::Direction::setRef(double tra) {
    ref = 1. - abs - tra;
    TestNum::snapToZero(ref);
    if (ref<0)
        ThrowException("Absorptivity + Transmission > 1").value1(abs).value2(tra);
}

//! Set absorptivity from transmissivity (assumes reflectivity already set)
void SurfaceRadiation::Spectrum::Direction::setAbs(double tra) {
    abs = 1. - ref - tra;
    TestNum::snapToZero(abs);
    Q_ASSERT(abs>=0.);
}

void SurfaceRadiation::Spectrum::Direction::check(double tra, QString name) {
    QString context = "SurfaceRadiation::Spectrum::Direction " + name;
    double sum = tra + abs + ref;
    if (TestNum::gt(sum, 1.))
        ThrowException("Light t+a+r>1 in " + context).value(sum);
    if (TestNum::gt(tra, 1.) || TestNum::ltZero(tra))
        ThrowException("Light transmission not inside [0;1] " + context).value(tra);
    if (TestNum::gt(abs, 1.) || TestNum::ltZero(abs))
        ThrowException("Light absorption not inside [0;1] " + context).value(abs);
    if (TestNum::gt(ref, 1.) || TestNum::ltZero(ref))
        ThrowException("Light reflection not inside [0;1] " + context).value(ref);
}

void SurfaceRadiation::Spectrum::check(QString name) {
    inner.check(tra, name+".inner");
    outer.check(tra, name+".outer");
}

//! Obtained combined characteristics of this and another Spectrum object
SurfaceRadiation::Spectrum& SurfaceRadiation::Spectrum::operator*=(const SurfaceRadiation::Spectrum &s2) {
    double k = 1. - inner.ref*s2.outer.ref,
           t12 = tra*s2.tra/k,
           r12_outer = outer.ref + sqr(tra)*s2.outer.ref,
           a12_outer = 1. - r12_outer - t12,

           r12_inner = s2.inner.ref + sqr(s2.tra)*inner.ref,
           a12_inner = 1. - r12_inner - t12;
    if (TestNum::eqZero(k)) {
        ThrowException("Two facing surfaces cannot both have 100% reflection");
    }
    tra = t12;
    inner.abs = a12_inner;
    inner.ref = r12_inner;
    outer.abs = a12_outer;
    outer.ref = r12_outer;
    return *this;
}

//! Obtained combined characteristics of this and another SurfaceRadiation object
SurfaceRadiation& SurfaceRadiation::operator*=(const SurfaceRadiation &s2) {
    light *= s2.light;
    directLight *= s2.directLight;
    lw *= s2.lw;
    return *this;
}

//! Check that the object holds valid characteristics
void SurfaceRadiation::check() {
    light.check("light");
    directLight.check("directLight");
    lw.check("kw");
}

} //namespace

