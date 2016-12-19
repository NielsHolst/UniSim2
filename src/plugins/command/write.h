#ifndef WRITE_H
#define WRITE_H
#include <base/command.h>

namespace command {

class write : public base::Command
{
public:
    write(QString name, QObject *parent);
private:
    // Methods
    void doExecute();
    void writeFile();
};

}

#endif
