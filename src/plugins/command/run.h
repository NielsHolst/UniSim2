#ifndef RUN_H
#define RUN_H
#include <base/command.h>

namespace command {

class run : public base::Command
{
public:
    run(QString name, QObject *parent);
    void execute();
};

}

#endif
