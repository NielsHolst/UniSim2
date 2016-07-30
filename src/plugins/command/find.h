#ifndef FIND_H
#define FIND_H
#include <QVector>
#include <base/command.h>

namespace command {

class find : public base::Command
{
public:
    find(QString name, QObject *parent);
private:
    // methods
    void doExecute();
};

}

#endif
