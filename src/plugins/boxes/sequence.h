/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class Sequence : public base::Box
{
public:
    Sequence(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    double min, max;
    QString by;
    QVector<double> values;
    // Outputs
    int counter, counterMax, offset;
    double value;
    // Data
    bool _useValues;
    // Methods
    void updateValue();
};

}

#endif
