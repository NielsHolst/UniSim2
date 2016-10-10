#ifndef LOAD_H
#define LOAD_H
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace command {

class load : public base::Command
{
public:
    load(QString name, QObject *parent);
private:
    // Methods
    void doExecute();
    void readFile(QString fileName);
    enum FileType {Boxes, Xml};
    FileType fileType(QString fileName);
};

}

#endif
