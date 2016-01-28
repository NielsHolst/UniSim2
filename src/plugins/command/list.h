#ifndef LIST_H
#define LIST_H
#include <base/command.h>

namespace command {

class list : public base::Command
{
public:
    list(QString name, QObject *parent);
    void execute(base::DialogBase *dialog, base::Environment *env);
};

}

#endif
