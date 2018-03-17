#ifndef CLIP_H
#define CLIP_H
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace command {

class clip : public base::Command
{
public:
    clip(QString name, QObject *parent);
private:
    // Methods
    void doExecute();
    void readFile(QString fileName);
    enum FileType {Boxes, Xml};
    FileType fileType(QString fileName);
};

}

#endif
