#ifndef SAVE_OUTPUT_H
#define SAVE_OUTPUT_H
#include <QFile>
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
private:
    // Data
    QVector<base::Port*> _ports;
    QFile _file;

    // Methods
    void doExecute();
    void collectPortsWithTrack();
    void writeOutput();
    void writeScript();
    void openFile(QString filePath);
    QString outputFilePath();
    QString scriptFilePath();
    QString scriptFolderPath();
    QString fileName();
    void writeTracks(QTextStream &stream);
    void writeColumnLabels(QTextStream &stream);
    void writeScriptCode(QTextStream &stream);
    QString keyOutputNumber();
    int getOutputNumber();
    void updateOutputNumber();
};

}

#endif
