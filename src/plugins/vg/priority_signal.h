/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PRIORITY_SIGNAL_H
#define PRIORITY_SIGNAL_H

#include "base_signal.h"

namespace vg {

class PrioritySignal : public BaseSignal
{
public:
    PrioritySignal(QString name, QObject *parent);
    void initialize();
    double computeSignal();
private:
    // Inputs
    bool reverseOrder;
    // Data
    QVector<const double*> _signals;
};

} //namespace
#endif
