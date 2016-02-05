#ifndef TEST_H
#define TEST_H
#include <base/command.h>

namespace command {

class test : public base::Command
{
public:
    test(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
