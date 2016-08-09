#ifndef RESET_H
#define RESET_H
#include <base/command.h>

namespace command {

class reset : public base::Command
{
public:
    reset(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
