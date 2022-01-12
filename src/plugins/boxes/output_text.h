/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H
#include <QFile>
#include <QList>
#include <QTextStream>
#include "output_ports.h"
#include "output_summary.h"

namespace base {
    class Track;
}

namespace boxes {

class OutputText : public OutputPorts
{
public:
    OutputText(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
    void cleanup();
    void debrief();
private:
    // Inputs
    bool skipFormats, useLocalDecimalChar,
         isActive;
    int step;
    QVector<QString> summary;

    // Outputs
    QString filePath;

    // Data
    bool _justChangedToActive;
    int _periodCount;
    OutputSummary _summary;
    struct {
        QVector<double> sum, min, max;
        QVector<int> minAt, maxAt;
    } _state;
    QFile _file;
    QTextStream _stream;

    // Methods
    void collectPaths();
    void setOutputNames();
    void initializeState();
    void openFileStream();
    void updateState();
    void processValues();
    void writeColumnLabels();
    void writeColumnFormats();
    void writeValues();
};

} // boxes

#endif
