/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H
#include <QFile>
#include <QList>
#include <QTextStream>
#include "output_ports.h"

namespace base {
    class Track;
}

namespace boxes {

class OutputText : public OutputPorts
{
public:
    OutputText(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
    void cleanup();
    void debrief();
private:
    // Inputs
    bool skipFormats;
    int skipInitialRows, averageN;
    // Data
    bool _isCondensed;
    int _rowCount;
    QVector<double> _tracksRunningSum, _tracksSum, _tracksMin, _tracksMax;
    QFile _file;
    QTextStream _stream;
    // Methods
    void openFileStream();
    void writeColumnLabels();
    void writeColumnFormats();
    void processValues();
};

} // boxes

#endif
