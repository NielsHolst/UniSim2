#ifndef QUIT_H
#define QUIT_H
#include <base/command.h>

namespace command {

class quit : public base::Command
{
public:
    quit(QString name, QObject *parent);
    void execute(base::DialogBase *dialog, base::Environment *env);
};

}

#endif
