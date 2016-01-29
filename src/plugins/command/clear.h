#ifndef CLEAR_H
#define CLEAR_H
#include <base/command.h>

namespace command {

class clear : public base::Command
{
public:
    clear(QString name, QObject *parent);
    void execute();
};

}

#endif
