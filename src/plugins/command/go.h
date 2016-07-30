#ifndef GO_H
#define GO_H
#include <QVector>
#include <base/command.h>

namespace command {

class go : public base::Command
{
public:
    go(QString name, QObject *parent);
private:
    // data
    // methods
    void doExecute();
};

}

#endif
