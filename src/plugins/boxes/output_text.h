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
    void debrief();
private:
    // Inputs
    bool skipFormats;
    int skipInitialRows, averageN;
    // Data
    QFile _file;
    QTextStream _stream;
    // Methods
    void openFileStream(QString extension);
    void writeDataFrame();
    void checkBufferSizes();
    void writeColumnLabels();
    void writeColumnFormats();
    void writeColumnValues();
    QStringList getRowValues(int row);
    QStringList getAverageRowValues(int firstRow);
    QString getAverageTrackValue(base::Track *track, int firstRow);
};

} // boxes

#endif
