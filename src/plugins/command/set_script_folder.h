#ifndef SET_SCRIPT_FOLDER_H
#define SET_SCRIPT_FOLDER_H
#include <QDir>
#include <base/command.h>

namespace command {

class set_script_folder : public base::Command
{
public:
    set_script_folder(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
