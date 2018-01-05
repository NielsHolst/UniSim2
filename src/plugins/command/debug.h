#ifndef DEBUG_H
#define DEBUG_H
#include <base/command.h>

namespace command {

class debug : public base::Command
{
public:
    debug(QString name, QObject *parent);
private:
    // Methods
    void doExecute();
};

}

#endif
