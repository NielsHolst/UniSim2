#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QDir>
#include <QObject>

namespace base {

class Box;

class Environment : public QObject {
public:
    Environment();
    struct {
        QDir dir;
        Box *boxRoot;
    } state;
private:
    static Environment *_environment;
    friend Environment& environment();
};

Environment& environment();

}

#endif
