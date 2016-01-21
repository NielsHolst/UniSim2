#ifndef COMMAND_H
#define COMMAND_H
#include <QObject>
#include <QStringList>

namespace console {
    class Environment;
}
namespace command {

class Command : public QObject
{
public:
    Command(QString name, QObject *parent = 0);

    void arguments(QStringList args)
        { _args = args; }

    QStringList arguments() const
        { return _args; }

    virtual void execute(console::Environment *env) = 0;

protected:
    void checkArgsNumber(int min, int max);
    QStringList _args;
};

}

#endif
