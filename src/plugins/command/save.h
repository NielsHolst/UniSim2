#ifndef SAVE_H
#define SAVE_H
#include <QFile>
#include <QSet>
#include <QTextStream>
#include <base/command.h>

namespace base {
    class Box;
}

namespace command {

class save : public base::Command
{
public:
    save(QString name, QObject *parent);
private:
    // methods
    void doExecute();
    QString filePath();
    void openFileStream();
    void writeBeginning();
    void writePlugins();
    void writeClasses();
    void writePorts();
    QSet<QString> collectPortNames(base::Box *root);
    void writePattern(QStringList match, QString name);
    void writeEnd();
    void closeFileStream();
    // data
    QFile _file;
    QTextStream _stream;

};

}

#endif
