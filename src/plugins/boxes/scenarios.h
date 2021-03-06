/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <QVector>
#include <base/box.h>
#include <base/data_frame.h>

namespace base {
class DataFrame;
}

namespace boxes {

class Scenarios : public base::Box
{
public:
    Scenarios(QString name, QObject *parent);
    void amend();
    void initialize();
    void reset();

private:
    // inputs
    QString fileName, title;
    // outputs
    QVector<QString> values;
    bool atEnd;
    // methods
    void readDataFrame();
    void createColumnOutputs();
    void copyValues();
    // data
    base::DataFrame _df;
    int _ixRow;
};

}
#endif
