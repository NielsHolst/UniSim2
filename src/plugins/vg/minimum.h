/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MINIMUM_H
#define MINIMUM_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class Minimum : public base::Box
{
public: 
    Minimum(QString name, QObject *parent=0);
    // standard methods
    void reset();
    void update();
private:
    QVector<double> values;
    double value;
};

} //namespace
#endif
