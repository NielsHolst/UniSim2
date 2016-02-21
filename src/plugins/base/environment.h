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
    QString outputFilePath(QString extension);
    void incrementFileCounter();
    void copyToClipboard(QString text);

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
    // Methods
    QString fileCounterKey();
    int fileCountervalue();
    static QDir makeDir(QDir baseDir, QDir specificDir);
};

Environment& environment();

}

#endif
