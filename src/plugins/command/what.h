#ifndef WHAT_H
#define WHAT_H
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace command {

class what : public base::Command
{
public:
    what(QString name, QObject *parent);
private:
    // Methods
    void doExecute();
    void readFile(QString fileName);
    enum FileType {Boxes, Xml};
    FileType fileType(QString fileName);
};

}

#endif
