#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H
#include <QFile>
#include <QList>
#include <QTextStream>
#include "output_ports.h"

namespace base {
    class Port;
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
    // Data
    QFile _file;
    QTextStream _stream;
    // Methods
    void writeDataFrame();
    void openFileStream(QString extension);
};

} // boxes

#endif
