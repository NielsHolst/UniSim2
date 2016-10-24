#ifndef EDIT_H
#define EDIT_H
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace command {

class edit : public base::Command
{
public:
    edit(QString name, QObject *parent);
private:
    // Methods
    void doExecute();
    void readFile(QString fileName);
};

}

#endif
