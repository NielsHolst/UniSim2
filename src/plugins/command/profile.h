#ifndef PROFILE_H
#define PROFILE_H
#include <base/command.h>

namespace command {

class profile : public base::Command
{
public:
    profile(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
