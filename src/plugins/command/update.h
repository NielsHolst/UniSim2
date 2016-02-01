#ifndef UPDATE_H
#define UPDATE_H
#include <base/command.h>

namespace command {

class update : public base::Command
{
public:
    update(QString name, QObject *parent);
    void execute();
};

}

#endif
