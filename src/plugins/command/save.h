#ifndef SAVE_H
#define SAVE_H
#include <base/command.h>

namespace base {
    class Box;
}

namespace command {

class save : public base::Command
{
public:
    save(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
