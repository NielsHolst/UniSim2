/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_RUNNING_AVERAGE_H
#define VG_RUNNING_AVERAGE_H

#include <QVector>
#include <base/box.h>

namespace vg {

class RunningAverage : public base::Box
{
public:
    RunningAverage(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double resetValue, input;
    int length;

    // Output
    double value;

    // Data
    QVector<double> buffer;
    double sum;
    int ix;
};
} //namespace


#endif
