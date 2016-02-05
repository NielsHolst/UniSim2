#ifndef SET_H
#define SET_H
#include <base/command.h>

namespace command {

class set : public base::Command
{
public:
    set(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
