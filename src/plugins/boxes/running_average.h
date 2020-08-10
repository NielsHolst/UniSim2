/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RUNNING_AVERAGE_H
#define RUNNING_AVERAGE_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class RunningAverage : public base::Box
{
public: 
    RunningAverage(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double input, initial;
    int count;
    // Output
    double value;
    // Data
    QVector<double> buffer;
    double sum;
    int pos;
};

} //namespace
#endif
