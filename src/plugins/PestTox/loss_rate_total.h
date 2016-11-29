/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LOSS_RATE_TOTAL_H
#define LOSS_RATE_TOTAL_H

#include <base/box.h>

namespace PestTox {

class LossRateTotal : public base::Box
{
public:
    LossRateTotal(QString name, QObject *parent);
    void amend();
    void update();

private:
    // Input
    QString path;
    double amount;

    // Output
    double fractionLost, amountLost;
    QVector<double> _outputRates, _outputAmounts;

    // Data
    QVector<const double*> _inputs;
};

} //namespace


#endif
