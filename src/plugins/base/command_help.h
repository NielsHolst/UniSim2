#ifndef COMMAND_HELP_H
#define COMMAND_HELP_H
#include <QString>
#include <QMap>
#include <QVector>

#define HELP(id, command, text) base::CommandHelp id##CommandHelp(command, text);

namespace base {

class CommandHelp {
public:
    CommandHelp(){}
    CommandHelp(QString command, QString description);
    QString command() const;
    QString description() const;
    static QVector<CommandHelp> all();
    static int maxWidth();
private:
    QString _command, _description;
    static QVector<CommandHelp> _all;
};

}

#endif
