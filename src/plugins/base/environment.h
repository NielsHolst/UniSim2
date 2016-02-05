#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QDir>
#include <QObject>

namespace base {

class Box;
class Command;

class Environment : public QObject {
public:
    Environment();
    ~Environment();
    struct {
        struct {
            QDir work, input, output, script;
        } dir;
        bool autosave;
        QString latestLoadArg, latestOutputFilePath;
        Box *root;
        Command *command;
    } state;
private:
    // Singleton
    static Environment *_environment;
    friend Environment& environment();
};

Environment& environment();

}

#endif
