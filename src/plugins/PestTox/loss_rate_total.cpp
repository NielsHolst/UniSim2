/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "loss_rate_total.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (LossRateTotal)
	
LossRateTotal::LossRateTotal(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(amount).imports("compartments/crop[load]").help("Amount from which losses are taken");
    Input(path).equals("../potential/*[finite]").help("Path to loss rates (d-1)");
    Output(fractionLost).help("Fraction lost calculated from loss rates on path (d-1)");
    Output(amountLost).help("Amount lost (d-1)");
}

void LossRateTotal::amend() {
    // Find input ports
    QVector<Port*> ports = resolveMany<Port>(path);

    // Collect pointers to port values;
    _inputs.clear();
    for (Port *port : ports)
        _inputs << port->valuePtr<double>();

    // Create output ports
    _outputRates.resize(ports.size());
    _outputAmounts.resize(ports.size());
    double *outputRate = _outputRates.data(),
           *outputAmount = _outputAmounts.data();
    for (Port *port : ports) {
        QString name = port->parent()->objectName();
        (*new Port(name+"Rate", this)).data(outputRate++).access(Port::Output).zeroAtReset();
        (*new Port(name+"Amount", this)).data(outputAmount++).access(Port::Output).zeroAtReset();
    }
}

void LossRateTotal::update() {
    // Compute total loss
    double remaining(1),
           totalInputs(0);
    for (const double *input : _inputs) {
        remaining *= (1. - *input);
        totalInputs += *input;
    }
    fractionLost = 1. - remaining;
    amountLost = fractionLost*amount;

    // Divide losses among rates
    double *outputRate = _outputRates.data(),
           *outputAmount = _outputAmounts.data();
    for (const double *input : _inputs) {
        double rate = *input/totalInputs * fractionLost;
        *outputRate++ = rate;
        *outputAmount++ = rate*amount;
    }

}


} //namespace

