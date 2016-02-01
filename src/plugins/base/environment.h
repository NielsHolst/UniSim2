#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QDir>
#include <QObject>

namespace base {

class Box;

class Environment : public QObject {
public:
    Environment();
    ~Environment();
    struct {
        QDir dir;
        QString outputFolder;
        Box *root;
    } state;
private:
    // Methods
    void setDir();
    void setOutputFolder();
    // Singleton
    static Environment *_environment;
    friend Environment& environment();
};

Environment& environment();

}

#endif
