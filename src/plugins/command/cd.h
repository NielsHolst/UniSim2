#ifndef CD_H
#define CD_H
#include <QDir>
#include <base/command.h>

namespace command {

class cd : public base::Command
{
public:
    cd(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif