#ifndef RECONFIGURE_H
#define RECONFIGURE_H
#include <QDir>
#include <base/command.h>

namespace command {

class reconfigure : public base::Command
{
public:
    reconfigure(QString name, QObject *parent);
    static QDir source();
    static QDir destination();
private:
    void doExecute();
};

}

#endif
