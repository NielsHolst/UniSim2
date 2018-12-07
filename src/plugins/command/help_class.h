#ifndef HELP_CLASS_H
#define HELP_CLASS_H
#include <QStringList>
#include <base/command.h>
#include <base/port.h>
#include <base/port_access.h>

namespace base {
    class Box;
    class FactoryPlugIn;
}

namespace command {

class help_class : public base::Command
{
public:
    help_class(QString name, QObject *parent);
private:
    // data
    const base::FactoryPlugIn* _plugIn;
    base::Box *_box;
    int _colWidthName, _colWidthValue;
    bool _expand;
    // methods
    void doExecute();
    bool getPlugIn(QString name);
    void writePlugInHelp();

    bool createBox(QString className);
    void setColWidths();
    void writeClassHelp();
    QStringList portsHelp(base::PortAccess access);
    QString sideEffects();
};

}

#endif
