/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AVERAGE_H
#define AVERAGE_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class Average : public base::Box
{
public: 
    Average(QString name, QObject *parent=0);
    // standard methods
    void initialize();
    void update();
private:
    QVector<QString> inputs;
    double value;
    QVector<base::Port*> ports;
};

} //namespace
#endif
