#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <QDir>

namespace boxes {
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
        boxes::Box *boxRoot;
    } state;
};

}

#endif
