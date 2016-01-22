#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <QDir>

namespace base {
    class Box;
}

namespace command {
    class Command;
}
namespace console {

class Environment {
public:
    Environment();
    void execute(command::Command *com);
    struct {
        QDir workingDirectory;
        base::Box *boxRoot;
    } state;
};

}

#endif
