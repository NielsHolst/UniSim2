#ifndef LION_H
#define LION_H
#include "command.h"

namespace command {

class Setwd : public Command
{
public:
    Setwd(QString name, QObject *parent);
    void test2() {}
    void execute(console::Environment *env);
};

}

#endif
