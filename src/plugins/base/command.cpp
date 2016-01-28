#include "exception.h"
#include "command.h"

namespace base {

Command::Command(QString name, QObject *parent)
    : QObject(parent)
{
    setObjectName(name);
}

void Command::checkArgsNumber(int min, int max) {
    int n = _args.size();
    if (n<min || n>max) {
        QString msg = (min == max) ?
                    QString("Command '%1' takes %2 arguments. Got %3.").
                    arg(objectName()).arg(min).arg(n)
                    :
                    QString("Command '%1' takes between %2 and %3 arguments. Got %4.").
                    arg(objectName()).arg(min).arg(max).arg(n);

        throw Exception(msg, "", this);
    }
}

}
