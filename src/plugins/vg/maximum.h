/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MAXIMUM_H
#define MAXIMUM_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class Maximum : public base::Box
{
public: 
    Maximum(QString name, QObject *parent=0);
    // standard methods
    void reset();
    void update();
private:
    QVector<double> values;
    double value;
};

} //namespace
#endif
