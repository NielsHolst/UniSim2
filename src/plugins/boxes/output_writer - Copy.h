/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_H
#define OUTPUT_H
#include <QFile>
#include <QList>
#include <QTextStream>
#include <base/box.h>

namespace base {
    class Track;
}

namespace boxes {

class Output : public base::Box
{
public:
    Output(QString name, base::Box *parent);
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
    struct {
        QVector<double> sum, min, max;
        QVector<int> minAt, maxAt;
    } _state;
    QFile _file;
    QTextStream _stream;

    // Methods
    void complementPorts();
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
