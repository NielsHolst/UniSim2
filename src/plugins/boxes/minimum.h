/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MINIMUM_H
#define MINIMUM_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class Minimum : public base::Box
{
public: 
    Minimum(QString name, QObject *parent);
    // standard methods
    void reset();
    void update();
private:
    QVector<double> values;
    double value;
    int count;
};

} //namespace
#endif
