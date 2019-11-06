/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PRIORITY_SIGNAL_H
#define PRIORITY_SIGNAL_H

#include <QVector>
#include <base/base_signal.h>

namespace boxes {

class PrioritySignal : public base::BaseSignal
{
public:
    PrioritySignal(QString name, QObject *parent);
    void initialize();
    bool computeFlag();
    double computeSignal(bool flag);
private:
    // Inputs
    bool reverseOrder;
    // Data
    struct FlaggedSignal {
        const bool *flag;
        const double *signal;
    };
    QVector<FlaggedSignal> _flaggedSignals;
};

} //namespace
#endif
