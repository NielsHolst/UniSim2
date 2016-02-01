#ifndef SAVE_OUTPUT_H
#define SAVE_OUTPUT_H
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace base {
    class Port;
}

namespace command {

class save_output : public base::Command
{
public:
    save_output(QString name, QObject *parent);
    void execute();
private:
    // Data
    QVector<base::Port*> _ports;

    // Methods
    bool collectPortsWithTrack();
    bool writeTracks(QTextStream &output);
};

}

#endif
