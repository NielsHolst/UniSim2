/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "distribution_generator_base.h"
#include "exception.h"

namespace base {

DistributionGeneratorBase::DistributionGeneratorBase(QString name, QObject *parent)
    : QObject(parent), _distribution(Uniform), _port(nullptr)
{
    Class(DistributionBase);
    setObjectName(name);
}

void DistributionGeneratorBase::port(Port *port_) {
    _port = port_;
}

Port* DistributionGeneratorBase::port() {
    if (!_port)
        ThrowException("No parent port for distribution").
            value(objectName()).context(this);
    return _port;
}

void DistributionGeneratorBase::distribution(QString distribution_) {
    QString s = distribution_.toLower();
    if (s == "uniform")
        _distribution = Uniform;
    else if (s == "normal")
        _distribution = Normal;
    else
        ThrowException("Unknown distribution")
                .value(distribution_)
                .hint("Choose 'Uniform' or 'Normal'")
                .context(this);
}

void DistributionGeneratorBase::distribution(Distribution distribution_) {
    _distribution = distribution_;
}

Distribution DistributionGeneratorBase::distribution() const {
    return _distribution;
}

}
