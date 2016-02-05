#ifndef COMMAND_H
#define COMMAND_H
#include <QObject>
#include <QStringList>

namespace base {

class Command : public QObject
{
public:
    Command(QString name, QObject *parent = 0);
    void arguments(QStringList args);
    QStringList arguments() const;
    void execute();
    bool hasError() const;
    static QStringList help();

protected:
    QStringList _args;
    void helpText(QString help);

private:
    virtual void doExecute() = 0;
    static QStringList _help;
    bool _error;
};

}

#endif
