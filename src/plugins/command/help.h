#ifndef HELP_H
#define HELP_H
#include <base/command.h>

namespace command {

class help : public base::Command
{
public:
    help(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
