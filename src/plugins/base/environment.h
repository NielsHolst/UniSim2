#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QDir>

namespace base {

class Box;

class Environment {
public:
    Environment();
    struct {
        QDir dir;
        Box *boxRoot;
    } state;
};

}

#endif
