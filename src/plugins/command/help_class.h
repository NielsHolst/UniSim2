#ifndef HELP_CLASS_H
#define HELP_CLASS_H
#include <QStringList>
#include <base/command.h>

namespace base {
    class Box;
    class Port;
}

namespace command {

class help_class : public base::Command
{
public:
    help_class(QString name, QObject *parent);
private:
    // data
    base::Box *_box;
    int _maxPortNameLength, _maxPortTypeNameLength;
    // methods
    void doExecute();
    void createBox(QString className);
    void setNameLengths();
    void writeHelp();
    QStringList portsHelp(base::Port::Access access);
};

}

#endif
